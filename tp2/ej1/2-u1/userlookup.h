/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _USERLOOKUP_H_RPCGEN
#define _USERLOOKUP_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef char *username;

#define ULKUP_PROG 555555556
#define ULKUP_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define byname 1
extern  int * byname_1(username *, CLIENT *);
extern  int * byname_1_svc(username *, struct svc_req *);
#define bynum 2
extern  username * bynum_1(int *, CLIENT *);
extern  username * bynum_1_svc(int *, struct svc_req *);
extern int ulkup_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define byname 1
extern  int * byname_1();
extern  int * byname_1_svc();
#define bynum 2
extern  username * bynum_1();
extern  username * bynum_1_svc();
extern int ulkup_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_username (XDR *, username*);

#else /* K&R C */
extern bool_t xdr_username ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_USERLOOKUP_H_RPCGEN */
