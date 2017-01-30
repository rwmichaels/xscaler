#define VERSION "V1.0V767"

#define HALLA 1

/*

Pseudo scaler generated by counting hits in V767 TDCs

*/

#include <stdlib.h>
#include <stdio.h>
#include <sysLib.h>
#include <sysSymTbl.h>
#include <taskLib.h>

#define VX_FP_TASK 0x0008  /* What do we really need here??? */


#define V767SIZE 128


unsigned int *scalerBuffer;
unsigned int *scalerOverflows;
unsigned int *eorBuffer;
unsigned int *eorOverflows;
int scalerRunInProgress=0;
int scalerBufferSize=0;
int scalerServerExists=0;
int scaler_event_counter=0;
int eor_scaler_event_counter=0;
int scalerReadInProgress=0;
int scalerClearInProgress=0;

struct V767SCALER {
  unsigned int vxTime0; /* vxWorks time at which scaling started */
  unsigned int vxTime;  /* Current vxWorks time */
  int nevents;    /* Number of events */
  int window_sum; /* Accumlated window in ns */
  int errcnt;
  int *nhitslist;
  int hitlist_size;
  int *nerrorslist;
  int errorlist_size;
};
typedef struct V767SCALER V767SCALER;

static int num_v767modules;

static int num_scalers;
static int num_sscalers;
static int num_vscalers;
int scaler_readout_period;
static void *base_scal_addr;
static void *base_sscal_addr;
void *lladdr;

long scaler_readout_killer_flag;

void start_scaler_task();
void set_runstatus(int status);
void start_f1readout_task();

void scalersEor()
     /* Copy the scalers into the end of run buffer */
{
  int i;
  for(i=0;i<scalerBufferSize;i++) {
    eorBuffer[i] = scalerBuffer[i];
    eorOverflows[i] = scalerOverflows[i];
  }
  eor_scaler_event_counter = scaler_event_counter;
  set_runstatus(0);
}
    
void scalersSend(V767SCALER *v767scaler)
     /* Called from event list every event.
	Ignore except when a second or 500 events has passed
     */
{
  double timedif;
  int tickdif;
  int nchan;
  int i,ioff;
  double ratio;
  unsigned int temp;

  set_runstatus(1);

  tickdif=(v767scaler->vxTime-v767scaler->vxTime0);
  if(tickdif < 60 && v767scaler->nevents < 500) {
    return;
  }
  timedif = tickdif/60.0;
  ratio = timedif/(v767scaler->window_sum/1000000000.);
/*  printf("Sum = %d, time=%f, ratio=%f\n",v767scaler->window_sum,timedif,ratio);*/
  v767scaler->window_sum = 0;
  v767scaler->vxTime0 = v767scaler->vxTime;

  nchan = num_v767modules*V767SIZE;
  if(v767scaler->hitlist_size < nchan) nchan=v767scaler->hitlist_size;

  for(i=0;i<nchan;i++) {
    temp = scalerBuffer[i];
    scalerBuffer[i] += v767scaler->nhitslist[i]*ratio;
    if(scalerBuffer[i] < temp) scalerOverflows[i]++;
    v767scaler->nhitslist[i]=0;
  }
  /* Copy error counts, don't scale up by ratio */
  ioff = num_v767modules*V767SIZE;
  nchan = num_v767modules;
  if(v767scaler->errorlist_size < nchan) nchan=v767scaler->errorlist_size;
  for(i=0;i<nchan;i++) {
    temp = scalerBuffer[i+ioff];
    scalerBuffer[i+ioff] += v767scaler->nerrorslist[i];
    if(scalerBuffer[i+ioff] < temp) scalerOverflows[i+ioff]++;
    v767scaler->nerrorslist[i]=0;
  }
  
  i = ioff + num_v767modules;

  temp =  scalerBuffer[i];
  scalerBuffer[i]+=v767scaler->nevents;
  if(scalerBuffer[i] < temp) scalerOverflows[i]++;
  v767scaler->nevents=0;
  scaler_event_counter = scalerBuffer[i];
  
  i++;
  temp = scalerBuffer[i];
  scalerBuffer[i]+=tickdif;
  if(scalerBuffer[i] < temp) scalerOverflows[i]++;


}
  

void setup_scalers(int nv767,int between_runs)
{
  int jj,res;
  char *S_ADDR, *SS_ADDR;


  void *scal_addr;
  char *s;
  void (*pscaSrvr)();
  SYM_TYPE symtype;
  STATUS symstat;
  int i;

  daLogMsg("INFO","Setup scalers.  Version %s @%d\n",VERSION,tickGet());


  num_v767modules = nv767;

  scalerBufferSize = V767SIZE*num_v767modules+num_v767modules+2;

  scalerBuffer = (unsigned int *) malloc(scalerBufferSize*sizeof(int));
  scalerOverflows = (unsigned int *) malloc(scalerBufferSize*sizeof(int));
  eorBuffer = (unsigned int *) malloc(scalerBufferSize*sizeof(int));
  eorOverflows = (unsigned int *) malloc(scalerBufferSize*sizeof(int));

  for(i=0;i<scalerBufferSize;i++) {
    scalerBuffer[i] = 0;
    scalerOverflows[i] = 0;
    eorBuffer[i] = 0;
    eorOverflows[i] = 0;
  }
  scaler_event_counter = 0;


  printf("Scaler buffer \"scalerBuffer\" of %d words alocated and initialized at 0x%x\n",scalerBufferSize,scalerBuffer);
  printf("Scaler overflow buffer \"scalerOverflows\" of %d words alocated and initialized at 0x%x\n",scalerBufferSize,scalerOverflows);
  scalerRunInProgress=0;        /* If !=0, then CODA is running */


  /* Check if the scaler server is loaded, and if so, start up both the
     scaler server and the between run's readout */
  symstat = symFindByName(sysSymTbl,"scaSrvr",(char **)&pscaSrvr,&symtype);
  if(symstat == 0) {
    
    /* Start the scaler server */
    daLogMsg("INFO","Starting scaler server");

    taskSpawn("scSrvr",140,VX_FP_TASK,20000,(FUNCPTR) *pscaSrvr,0,0,0,0,0,0,0,0,0,0);

  }

  if(between_runs) {
    start_f1readout_task();
  }

}

/* Return the number of channels of scalers modules */
int get_num_scalers()
{
  return(scalerBufferSize);
}

void set_runstatus(int status)
{
  scalerRunInProgress = status;
}

int get_runstatus()
{
  return(scalerRunInProgress);
}


/* Copy scalers into array created by scalers server.  Assume that the
   scaler server made first and count sensible values */
struct VALUE {
	int val;
	int ovf;
};
typedef struct VALUE VALUE;

int scalers_copy(VALUE *values, int first, int count)
{
  int i;

  for(i=0;i<count;i++) {
    values[i].val = scalerBuffer[first+i];
    values[i].ovf = scalerOverflows[first+i];
    /*    printf("%d: %d %d\n", first+i, values[i].val, values[i].ovf);*/
  }
  return(scaler_event_counter);
}

int eor_scalers_copy(VALUE *values, int first, int count)
{
  int i;

  for(i=0;i<count;i++) {
    values[i].val = eorBuffer[first+i];
    values[i].ovf = eorOverflows[first+i];
    /*    printf("%d: %d %d\n", first+i, values[i].val, values[i].ovf);*/
  }
  return(eor_scaler_event_counter);
}

void kill_v767scaler_task()
{
  scaler_readout_killer_flag=1;
}

const TDC_ID1 = 0;
const V767_TW_OFF = 1200;
const V767_TW_WID = 1000;

/* Use F1TDC library global variables */
const MAX_V767_DATA = 1024;

void v767ReadoutTask() {
  /* Software trigger the F1 TDC's and accumulate hit counts from each
     channel.  Watch for a flag to be set by the readout list telling
     this to give up.
  */

  int i,ii;
  unsigned int v767data[MAX_V767_DATA];
  unsigned int stat;
  static struct V767SCALER v767scaler = {0,0,0,0,0,0,0,0,0};
  unsigned int dataword;
  int slot, itdc, nwrds, chan, headertrailer;
  int v767_Window=2000;

  scaler_readout_killer_flag=0;

  /* Create structure to hold accumlated data */


  v767scaler.vxTime0=tickGet();
  v767scaler.nevents = 0;
  v767scaler.window_sum = 0;
  v767scaler.errcnt = 0;
  v767scaler.hitlist_size = num_v767modules*V767SIZE;
  v767scaler.nhitslist = (int *) malloc(v767scaler.hitlist_size*sizeof(int));
  v767scaler.errorlist_size = num_v767modules;
  v767scaler.nerrorslist = (int *) malloc(v767scaler.hitlist_size*sizeof(int));
  for(i=0;i<v767scaler.hitlist_size;i++) {
    v767scaler.nhitslist[i] = 0;
  }
  for(i=0;i<v767scaler.errorlist_size;i++) {
    v767scaler.nerrorslist[i] = 0;
  }
  /* Program the TDCs */
  for (ii=TDC_ID1;ii<TDC_ID1+num_v767modules;ii++) {
    /* Reset v767 TDC and wait */
    if(scaler_readout_killer_flag) goto fooy;
    v767AutoloadDisable(ii);
  }
  for (ii=TDC_ID1;ii<TDC_ID1+num_v767modules;ii++) {
    /* select stop trigger matching, substraction of trigger time,
       all channels on, autoload on, and save configuration */
    if(scaler_readout_killer_flag) goto fooy;

    v767SetAcqMode(ii,0);
    v767SetDReadyMode(ii,0);

    v767OverlapTrigDisable(ii);
    v767TriggerConfig(ii,v767_Window,-(v767_Window+200),0);

  }
  for (ii=TDC_ID1;ii<TDC_ID1+num_v767modules;ii++) {
    /* Clear v767 TDC */
    if(scaler_readout_killer_flag) goto fooy;
    v767Clear(ii);
  }  

  while(!scaler_readout_killer_flag) {
    for(i=TDC_ID1;i<TDC_ID1+num_v767modules;i++) {
      v767Trig(i);		/* Software trigger */
    }
    taskDelay(1);
    for(itdc=TDC_ID1;itdc<TDC_ID1+num_v767modules;itdc++) {
      if(v767Overflow(itdc)!=0) {
	v767scaler.nerrorslist[itdc-TDC_ID1]++;
	v767Clear(itdc);
      } else {
	nwrds = v767ReadEvent(itdc,v767data,MAX_V767_DATA);
	/*	printf("TDC %d nwrds=%d\n",itdc,nwrds);*/
	if(nwrds <= 0) {
	  v767scaler.nerrorslist[itdc-TDC_ID1]++;
	  printf("TDC %d: nwrds=%d\n",ii,nwrds);
	} else {
	  for(ii=0;ii<nwrds;ii++) {
	    dataword = v767data[ii];
	    headertrailer=dataword&0x00600000;
	    if(!headertrailer) {
	      chan = (dataword>>24)&0x7f;
	      if(itdc-TDC_ID1<num_v767modules && itdc>=TDC_ID1 && chan < 128) {
		v767scaler.nhitslist[(itdc-TDC_ID1)*V767SIZE+chan]++;
	      } else {
		printf("Bad slot/chan %d %d\n",itdc,chan);
	      }
	    }
	  }
	}
      }
    }
    v767scaler.vxTime=tickGet();
    v767scaler.nevents++;
    v767scaler.window_sum += v767_Window;
    scalersSend(&v767scaler);
  }	

 fooy:
  free(v767scaler.nhitslist);
  free(v767scaler.nerrorslist);

}

void start_f1readout_task()
{

  daLogMsg("INFO","Starting in-between run reading of V767 TDC's");
  taskSpawn("v767scaler",115,VX_FP_TASK,20000,(FUNCPTR) *v767ReadoutTask,0,0,0,0,0,0,0,0,0,0);
}

