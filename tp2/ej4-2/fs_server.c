#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


buffer *
read_file_1_svc(filename filename, int offset, int bytes,  struct svc_req *rqstp)
{
	static buffer result;
	FILE *file;

	file = fopen(filename, "rb");
	if (file == NULL) {
		return NULL;
	}

	lseek(file->_fileno, offset, SEEK_SET);
	result.buffer_val = malloc(bytes * sizeof(char));
	result.buffer_len = read(file->_fileno, result.buffer_val, bytes);

	printf("read: %d bytes leidos del archivo %s\n", result.buffer_len, filename);
	
	fclose(file);
	return &result;
}


int *
write_file_1_svc(filename filename, buffer buffer, int bytes,  struct svc_req *rqstp)
{
	static int result;
	FILE *file;

	file = fopen(filename, "ab");
	if (file == NULL) {
		return 0;
	}

	result = fwrite(buffer.buffer_val, 1, buffer.buffer_len, file);
	
	printf("write: %d bytes escritos en el archivo %s\n", result, filename);

	fclose(file);
	return &result;
}
