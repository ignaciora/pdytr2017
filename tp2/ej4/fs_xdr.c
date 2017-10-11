/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "fs.h"

bool_t
xdr_buffer (XDR *xdrs, buffer *objp)
{
	register int32_t *buf;

	 if (!xdr_bytes (xdrs, (char **)&objp->buffer_val, (u_int *) &objp->buffer_len, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_filename (XDR *xdrs, filename *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, 255))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_read_ret (XDR *xdrs, read_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_buffer (xdrs, &objp->buffer))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->bytes_read))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->bytes_requested))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_read_args (XDR *xdrs, read_args *objp)
{
	register int32_t *buf;

	 if (!xdr_filename (xdrs, &objp->filename))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->bytes))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->offset))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_write_args (XDR *xdrs, write_args *objp)
{
	register int32_t *buf;

	 if (!xdr_filename (xdrs, &objp->filename))
		 return FALSE;
	 if (!xdr_buffer (xdrs, &objp->buffer))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->bytes))
		 return FALSE;
	return TRUE;
}