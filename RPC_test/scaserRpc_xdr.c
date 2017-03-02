/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "scaserRpc.h"

bool_t
xdr_SLOTINFO (XDR *xdrs, SLOTINFO *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->scalertype, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->nchans))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_INFO (XDR *xdrs, INFO *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->slots))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->channels))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->runinprogress))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->clock_channel))
				 return FALSE;

		} else {
		IXDR_PUT_LONG(buf, objp->slots);
		IXDR_PUT_LONG(buf, objp->channels);
		IXDR_PUT_LONG(buf, objp->runinprogress);
		IXDR_PUT_LONG(buf, objp->clock_channel);
		}
		 if (!xdr_array (xdrs, (char **)&objp->slotinfo.slotinfo_val, (u_int *) &objp->slotinfo.slotinfo_len, ~0,
			sizeof (SLOTINFO), (xdrproc_t) xdr_SLOTINFO))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->slots))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->channels))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->runinprogress))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->clock_channel))
				 return FALSE;

		} else {
		objp->slots = IXDR_GET_LONG(buf);
		objp->channels = IXDR_GET_LONG(buf);
		objp->runinprogress = IXDR_GET_LONG(buf);
		objp->clock_channel = IXDR_GET_LONG(buf);
		}
		 if (!xdr_array (xdrs, (char **)&objp->slotinfo.slotinfo_val, (u_int *) &objp->slotinfo.slotinfo_len, ~0,
			sizeof (SLOTINFO), (xdrproc_t) xdr_SLOTINFO))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_int (xdrs, &objp->slots))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->channels))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->runinprogress))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->clock_channel))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->slotinfo.slotinfo_val, (u_int *) &objp->slotinfo.slotinfo_len, ~0,
		sizeof (SLOTINFO), (xdrproc_t) xdr_SLOTINFO))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_GETREQUEST (XDR *xdrs, GETREQUEST *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->first))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->count))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_VALUE (XDR *xdrs, VALUE *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->val))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ovf))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_VALUES (XDR *xdrs, VALUES *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->first))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->count))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->read_counter))
				 return FALSE;

		} else {
		IXDR_PUT_LONG(buf, objp->first);
		IXDR_PUT_LONG(buf, objp->count);
		IXDR_PUT_LONG(buf, objp->read_counter);
		}
		 if (!xdr_array (xdrs, (char **)&objp->value.value_val, (u_int *) &objp->value.value_len, ~0,
			sizeof (VALUE), (xdrproc_t) xdr_VALUE))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->first))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->count))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->read_counter))
				 return FALSE;

		} else {
		objp->first = IXDR_GET_LONG(buf);
		objp->count = IXDR_GET_LONG(buf);
		objp->read_counter = IXDR_GET_LONG(buf);
		}
		 if (!xdr_array (xdrs, (char **)&objp->value.value_val, (u_int *) &objp->value.value_len, ~0,
			sizeof (VALUE), (xdrproc_t) xdr_VALUE))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_int (xdrs, &objp->first))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->count))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->read_counter))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->value.value_val, (u_int *) &objp->value.value_len, ~0,
		sizeof (VALUE), (xdrproc_t) xdr_VALUE))
		 return FALSE;
	return TRUE;
}