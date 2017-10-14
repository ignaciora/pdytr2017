#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#ifndef FILE_CHUNK_SIZE
	#define FILE_CHUNK_SIZE 255
#endif


/* Devuelve 0 si se logro extraer del server el archivo completo y 1 si se lo extrajo en forma parcial o si no se recupero nada. */
int fs_read(char *source_file, char *dest_file, CLIENT *clnt)
{
	FILE *file;
	buffer *file_part;
	int reads_count = 0;

	file = fopen(dest_file, "wb");
	if (file == NULL) {
		printf("read: (ERROR) El archivo %s no existe\n", source_file);
		return 1;
	}

	do {
		file_part = read_file_1((filename)source_file, reads_count * FILE_CHUNK_SIZE, FILE_CHUNK_SIZE, clnt);
		if (file_part == (buffer *) NULL) {
			clnt_perror (clnt, "read: (ERROR) Fallo la llamada al servidor\n");
			return 1;
		}

		if (file_part->buffer_len == -1) {
			printf("read: (ERROR) Fallo la lectura del archivo en el servidor\n");
			return 1;
		}

		fwrite(file_part->buffer_val, file_part->buffer_len, 1, file);
		reads_count++;
	}
	while (file_part->buffer_len == FILE_CHUNK_SIZE);

	fclose(file);
	return 0;
}


/* Devuelve 0 si se logro depositar en el server el archivo completo y 1 si se lo deposito en forma parcial o si no se guardo nada. */
int fs_write(char *source_file, char *dest_file, CLIENT *clnt)
{
	FILE *file;
	buffer *file_part;
	int *result;
	int written_bytes = 0;
	struct stat file_stat;

	file = fopen(source_file, "rb");
	if (file == NULL) {
		printf("write: (ERROR) El archivo %s no existe\n", source_file);
		return 1;
	}
	
	if (fstat(file->_fileno, &file_stat) == -1) {
		printf("write: (ERROR) No se pudo evaluar el archivo %s\n", source_file);
		return 1;
	}

	file_part = malloc(sizeof(buffer));
	file_part->buffer_val = malloc(FILE_CHUNK_SIZE * sizeof(buffer));

	do {
		lseek(file->_fileno, written_bytes, SEEK_SET);

		file_part->buffer_len = read(file->_fileno, file_part->buffer_val, FILE_CHUNK_SIZE);
		if (file_part->buffer_len == -1) {
			printf("write: (ERROR) No se pudo cargar el buffer a enviar con el archivo %s\n", source_file);
			return 1;
		}

		result = write_file_1((filename)dest_file, *file_part, file_part->buffer_len, clnt);
		if (result == (int *) NULL) {
			clnt_perror (clnt, "write: (ERROR) Fallo la llamada al servidor\n");
			return 1;
		}

		if (*result == 0) {
			printf("write: (ERROR) Fallo la escritura del archivo en el servidor\n");
			return 1;
		}

		written_bytes += *result;
	}
	while (file_stat.st_size > written_bytes);

	fclose(file);
	free(file_part);
	return 0;
}


void fs_prog_1(char *host, char opt, char *source_file, char *dest_file)
{
	CLIENT *clnt;

	clnt = clnt_create(host, FS_PROG, FS_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	switch (opt) {
		/* read */
		case 'r':
			if (fs_read(source_file, dest_file, clnt) != 0) {
				printf("read: No se pudo completar la operacion\n");
			}
			break;

		/* write */
		case 'w':
			if (fs_write(source_file, dest_file, clnt) != 0) {
				printf("write: No se pudo completar la operacion\n");
			}
			break;

		default:
			printf("(ERROR) No se reconocio la accion -%c\n", opt);
			break;
	}

	clnt_destroy(clnt);
}


/* Se trae el archivo source_file (server) a dest_file (client) y luego crea una copia de source_file en el servidor. */
void copy_gen(char *host, char *source_file, char *dest_file)
{
	CLIENT *clnt;
	
	clnt = clnt_create(host, FS_PROG, FS_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	if (fs_read(source_file, dest_file, clnt) != 0) {
		printf("read: No se pudo completar la operacion\n");
	}

	char *copy_file = malloc(strlen(source_file) + strlen("__copy") + 1);
	copy_file[0] = '\0'; //se asegura que la funcion tome la memoria como vacia
	strcat(copy_file, source_file);
	strcat(copy_file, "__copy");

	if (fs_write(dest_file, copy_file, clnt) != 0) {
		printf("write: No se pudo completar la operacion\n");
	}

	free(copy_file);
	clnt_destroy(clnt);
}


int main (int argc, char *argv[])
{
	if (argc < 5) {
		printf("usage: %s server_host (-r|-w|--copy) source_file dest_file\n", argv[0]);
		exit(1);
	}

	if (strcmp(argv[2], "--copy") == 0) {
		copy_gen(argv[1], argv[3], argv[4]);
	}
	else {
		fs_prog_1(argv[1], argv[2][1], argv[3], argv[4]);
	}

	exit(0);
}
