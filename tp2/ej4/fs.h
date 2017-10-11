/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _FS_H_RPCGEN
#define _FS_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	u_int buffer_len;
	char *buffer_val;
} buffer;

typedef char *filename;

struct read_ret {
	buffer buffer;
	long bytes_read;
	long bytes_requested;
};
typedef struct read_ret read_ret;

struct read_args {
	filename filename;
	long bytes;
	long offset;
};
typedef struct read_args read_args;

struct write_args {
	filename filename;
	buffer buffer;
	long bytes;
};
typedef struct write_args write_args;

#define FS_PROG 200000001
#define FS_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define READ 1
extern  enum clnt_stat read_1(read_args *, read_ret *, CLIENT *);
extern  bool_t read_1_svc(read_args *, read_ret *, struct svc_req *);
#define WRITE 2
extern  enum clnt_stat write_1(write_args *, long *, CLIENT *);
extern  bool_t write_1_svc(write_args *, long *, struct svc_req *);
extern int fs_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define READ 1
extern  enum clnt_stat read_1();
extern  bool_t read_1_svc();
#define WRITE 2
extern  enum clnt_stat write_1();
extern  bool_t write_1_svc();
extern int fs_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_buffer (XDR *, buffer*);
extern  bool_t xdr_filename (XDR *, filename*);
extern  bool_t xdr_read_ret (XDR *, read_ret*);
extern  bool_t xdr_read_args (XDR *, read_args*);
extern  bool_t xdr_write_args (XDR *, write_args*);

#else /* K&R C */
extern bool_t xdr_buffer ();
extern bool_t xdr_filename ();
extern bool_t xdr_read_ret ();
extern bool_t xdr_read_args ();
extern bool_t xdr_write_args ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_FS_H_RPCGEN */