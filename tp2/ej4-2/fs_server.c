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
		printf("read: (ERROR) Se pidio el archivo %s pero no se lo pudo abrir\n", filename);
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
		printf("write: (ERROR) No se pudo abrir o crear el archivo %s\n", filename);
		return 0;
	}

	result = fwrite(buffer.buffer_val, 1, buffer.buffer_len, file);
	
	printf("write: %d bytes escritos en el archivo %s\n", result, filename);

	fclose(file);
	return &result;
}
