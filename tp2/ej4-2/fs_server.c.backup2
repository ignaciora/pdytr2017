#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string.h>


bool_t read_1_svc(read_args *argp, read_ret *result, struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * insert server code here
	 */

	return retval;
}


bool_t write_1_svc(write_args *argp, long *result, struct svc_req *rqstp)
{
	bool_t retval = FALSE;
	result = 0;
	
	/* DEBUG */
	printf("write_svc: concat folder + filename: %s\n", strcat("file_storage/", argp->filename));
	
	FILE *file = fopen(strcat("file_storage/", argp->filename), "a");
	if (file == NULL) {
		perror("write_svc: (ERROR) No se pudo abrir o crear el archivo\n");
		return retval;
	}

	result = write(file->_fileno, &argp->buffer.buffer_val, argp->bytes);
	if (result == 0) {
		perror("write_svc: (ERROR) No se pudo escribir bytes del archivo\n");
		return retval;
	}

	printf("write_svc: Se escribieron %li bytes\n", result);
	fclose(file);
	retval = TRUE;

	return retval;
}


int fs_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free(xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
