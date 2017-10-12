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

	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("No se puede leer el archivo %s\n", filename);
		result.buffer_len = -1;
		return &result;
	}

	lseek(fd, offset, SEEK_SET);
	result.buffer_val = malloc(bytes * sizeof(char));
	result.buffer_len = read(fd, result.buffer_val, bytes);

	printf("Leidos %d bytes del archivo %s:\n%s\n", result.buffer_len, filename, result.buffer_val);
	
	close(fd);
	return &result;
}


int *
write_file_1_svc(filename filename, buffer buffer, int bytes,  struct svc_req *rqstp)
{
	static int result;

	int fd = open(filename, O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd == -1) {
		fd = creat(filename, O_WRONLY);
		fchmod(fd, 0b111101101);
	}

	result = write(fd, buffer.buffer_val, bytes);
	printf("Se escribieron %d bytes\n", result);

	close(fd);
	return &result;
}
