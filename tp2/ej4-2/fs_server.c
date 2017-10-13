#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


buffer *
read_file_1_svc(filename filename, int offset, int bytes,  struct svc_req *rqstp)
{
	static buffer result;

	FILE *fd;
	fd = fopen(filename, "rb");

	lseek(fd->_fileno, offset, SEEK_SET);
	result.buffer_val = malloc(bytes * sizeof(char));
	result.buffer_len = read(fd->_fileno, result.buffer_val, bytes);

	printf("Leidos %d bytes del archivo %s", result.buffer_len, filename);
	
	fclose(fd);
	return &result;
}


int *
write_file_1_svc(filename filename, buffer buffer, int bytes,  struct svc_req *rqstp)
{
	static int result;

	FILE *fd;
	fd = fopen(filename, "ab");

	result = fwrite(buffer.buffer_val, 1, buffer.buffer_len, fd);
	printf("Se escribieron %d bytes\n", result);

	fclose(fd);
	return &result;
}
