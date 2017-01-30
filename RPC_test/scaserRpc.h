/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SCASERRPC_H_RPCGEN
#define _SCASERRPC_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct SLOTINFO {
	char *scalertype;
	int nchans;
};
typedef struct SLOTINFO SLOTINFO;

struct INFO {
	int slots;
	int channels;
	int runinprogress;
	int clock_channel;
	struct {
		u_int slotinfo_len;
		struct SLOTINFO *slotinfo_val;
	} slotinfo;
};
typedef struct INFO INFO;

struct GETREQUEST {
	int first;
	int count;
};
typedef struct GETREQUEST GETREQUEST;

struct VALUE {
	int val;
	int ovf;
};
typedef struct VALUE VALUE;

struct VALUES {
	int first;
	int count;
	int read_counter;
	struct {
		u_int value_len;
		struct VALUE *value_val;
	} value;
};
typedef struct VALUES VALUES;

#define SCASERPROG 0x2c0da020
#define SCASERVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define SCASER_CLEAR 1
extern  int * scaser_clear_1(void *, CLIENT *);
extern  int * scaser_clear_1_svc(void *, struct svc_req *);
#define SCASER_GETINFO 2
extern  INFO * scaser_getinfo_1(void *, CLIENT *);
extern  INFO * scaser_getinfo_1_svc(void *, struct svc_req *);
#define SCASER_GETSCALERS 3
extern  VALUES * scaser_getscalers_1(GETREQUEST *, CLIENT *);
extern  VALUES * scaser_getscalers_1_svc(GETREQUEST *, struct svc_req *);
#define SCASER_GETEORSCALERS 4
extern  VALUES * scaser_geteorscalers_1(GETREQUEST *, CLIENT *);
extern  VALUES * scaser_geteorscalers_1_svc(GETREQUEST *, struct svc_req *);
extern int scaserprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SCASER_CLEAR 1
extern  int * scaser_clear_1();
extern  int * scaser_clear_1_svc();
#define SCASER_GETINFO 2
extern  INFO * scaser_getinfo_1();
extern  INFO * scaser_getinfo_1_svc();
#define SCASER_GETSCALERS 3
extern  VALUES * scaser_getscalers_1();
extern  VALUES * scaser_getscalers_1_svc();
#define SCASER_GETEORSCALERS 4
extern  VALUES * scaser_geteorscalers_1();
extern  VALUES * scaser_geteorscalers_1_svc();
extern int scaserprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_SLOTINFO (XDR *, SLOTINFO*);
extern  bool_t xdr_INFO (XDR *, INFO*);
extern  bool_t xdr_GETREQUEST (XDR *, GETREQUEST*);
extern  bool_t xdr_VALUE (XDR *, VALUE*);
extern  bool_t xdr_VALUES (XDR *, VALUES*);

#else /* K&R C */
extern bool_t xdr_SLOTINFO ();
extern bool_t xdr_INFO ();
extern bool_t xdr_GETREQUEST ();
extern bool_t xdr_VALUE ();
extern bool_t xdr_VALUES ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_SCASERRPC_H_RPCGEN */
