/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <stdio.h>
#include <stdlib.h> /* getenv, exit */
#include <rpc/rpc.h>

main(argc, argv)
	int argc;
	char *argv[];
{
	char *host;
	CLIENT *handle;
	int *scalers,*overflows;
	int channels,iclock,i;
	int halla_hex=0;
	int clear=0;

	if (argc < 2) {
		printf("usage:  %s [-x] [-c] server_host\n", argv[0]);
		exit(1);
	}
	argv++;
	while(*argv != NULL) {
	  if(strcmp(*argv,"-x")==0) {
	    halla_hex = 1;
	  } else if (strcmp(*argv,"-c") == 0) {
	    clear = 1;
	  } else {
	    host = *argv;
	  }
	  argv++;
	}
	  
	if(!(handle=scaserOpen(host))) {
	  fprintf(stderr,"Error connecting to %s\n",host);
	  exit(1);
	}
	if(clear) {
	  scaserClear(handle);
	}

	scaserGetInfo(handle,0,&channels,0,&iclock);

	scalers = (int *) malloc(channels*sizeof(int));
	overflows = (int *) malloc(channels*sizeof(int));
	
	scaserReadScalers(handle, 0, channels, scalers, overflows,0);
	for(i=0;i<channels;i++) {
	  if(halla_hex) {
	    if(i%32==0) {
	      int header, len;
	      if(channels-i < 32) {
		len = channels-i;
	      } else {
		len = 32;
	      }
	      header = 0xbba00000 + ((i/32)<<16) + len;
	      printf("%x\n",header);
	    }
	    printf("%x\n",scalers[i]);
	  } else {
	    printf("%d: %u(%u)\n",i,scalers[i],overflows[i]);
	  }
	}
	exit(0);
}
