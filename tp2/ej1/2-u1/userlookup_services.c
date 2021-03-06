/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <pwd.h>		/* needed for pw lookup functions */
#include <sys/types.h>          /* needed for pw database types */

#include "userlookup.h"

int * 
byname_1_svc(username *argp, struct svc_req *rqstp)
{
	static int  result;
	struct passwd *pw;

	/* argp is a pointer to: a string (a char *)! */
	pw = getpwnam( *argp );
	if (pw==NULL) {
	  /* ERROR ! NO USER FOUND */
	  result = -1;
	} else {
	  result = pw->pw_uid;
	}
	return(&result);
}

char *error="ERROR";
username * 
bynum_1_svc(int *argp, struct svc_req *rqstp)
{

	static username  result;
	struct passwd *pw;

	/* username is a pointer to: a string (a char *)! */
	pw = getpwuid( *argp );
	if (pw==NULL) {
	  /* ERROR ! NO USER FOUND */
	  /* WE CANNOT RETURN A NULL (will screw up xdr filter) */
	  /* Point to an error string */
	  result=error;
	} else {
	  result = pw->pw_name;
	}

	return(&result);
}






