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
	FILE *file;
	char *path;

	path = malloc(strlen("./fs/") + strlen(filename) + 1);
	path[0] = '\0'; //se asegura que la funcion tome la memoria como vacia
	strcat(path, "./fs/");
	strcat(path, filename);

	file = fopen(path, "rb");
	if (file == NULL) {
		printf("read: (ERROR) Se pidio el archivo %s pero no se lo pudo abrir\n", path);
		return NULL;
	}

	lseek(file->_fileno, offset, SEEK_SET);
	result.buffer_val = malloc(bytes * sizeof(char));
	result.buffer_len = read(file->_fileno, result.buffer_val, bytes);

	printf("read: %d bytes leidos del archivo %s\n", result.buffer_len, path);
	
	fclose(file);
	free(path);
	return &result;
}


int *
write_file_1_svc(filename filename, buffer buffer, int bytes,  struct svc_req *rqstp)
{
	static int result;
	FILE *file;
	char *path;
	
	path = malloc(strlen("./fs/") + strlen(filename) + 1);
	path[0] = '\0'; //se asegura que la funcion tome la memoria como vacia
	strcat(path, "./fs/");
	strcat(path, filename);

	file = fopen(path, "ab");
	if (file == NULL) {
		printf("write: (ERROR) Se recibio el archivo %s pero no se lo pudo crear o abrir\n", path);
		return 0;
	}

	result = fwrite(buffer.buffer_val, 1, buffer.buffer_len, file);
	
	printf("write: %d bytes escritos en el archivo %s\n", result, path);

	fclose(file);
	free(path);
	return &result;
}
