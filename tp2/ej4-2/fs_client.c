#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef FILE_CHUNK_SIZE
	#define FILE_CHUNK_SIZE 255
#endif


void fs_prog_1(char *host, char opt, char *source_file, char *dest_file)
{
	CLIENT *clnt;

	clnt = clnt_create(host, FS_PROG, FS_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit(1);
	}

	switch (opt) {
		/* read */
		case 'r':
			if (fs_read(source_file, dest_file, clnt) != 0) {
				printf("read: (ERROR) Fallo la lectura total o parcial del archivo que se encuentra en el servidor\n");
			}
			break;

		/* write */
		case 'w':
			if (fs_write(source_file, dest_file, clnt) != 0) {
				printf("write: (ERROR) Fallo la escritura total o parcial del archivo en el servidor\n");
			}
			break;

		default:
			printf("(ERROR) No se reconocio la accion -%c\n", opt);
			break;
	}

	clnt_destroy(clnt);
}


/* Devuelve 0 si se logro extraer del server el archivo completo y 1 si se lo extrajo en forma parcial o si no se recupero nada. */
int fs_read(char *source_file, char *dest_file, CLIENT *clnt)
{
	FILE *fd;
	buffer *file_part;
	int reads_count = 0;

	fd = fopen(dest_file, "wb");

	do {
		file_part = read_file_1((filename)source_file, reads_count * FILE_CHUNK_SIZE, FILE_CHUNK_SIZE, clnt);

		if (file_part == (buffer *) NULL) {
			clnt_perror (clnt, "read: (ERROR) Fallo la llamada al server\n");
			return 1;
		}

		if (file_part->buffer_len == -1) {
			printf("read: (ERROR) El archivo %s no existe\n.", source_file);
			return 1;
		}

		fwrite(file_part->buffer_val, file_part->buffer_len, 1, fd);

		reads_count++;
	}
	while (file_part->buffer_len == FILE_CHUNK_SIZE);

	fclose(fd);

	return 0;
}


/* Devuelve 0 si se logro depositar en el server el archivo completo y 1 si se lo deposito en forma parcial o si no se guardo nada. */
int fs_write(char *source_file, char *dest_file, CLIENT *clnt)
{
	/*
	Abrir un archivo
	Saber cuanto tengo que pasar en total
	Hasta que haya completado el tamaño del archivo:
		Levantar con lseek el cacho de archivo (CHUNK SIZE)
		Mandar al servidor el cacho de archivo
		Actualizo el tamaño de lo que llevo leyendo con lo que escribio el server
	
	Cierro el archivo
	*/
	printf("Funciono, luego existo");
	buffer *file_part;
	file_part = malloc(sizeof(buffer));
	file_part->buffer_val = malloc(FILE_CHUNK_SIZE * sizeof(buffer));

	int *result;
	int written_bytes = 0;
	struct stat file_stat;

	//Abrir un archivo
	FILE *fd;
	fd = fopen(source_file, "rb");
	
	//Saber cuanto tengo que pasar en total
	if (fstat(fd->_fileno, &file_stat) == -1) {
		return 1;
	}

	printf("Logre abrir el archivo y cargar los stats");

	//Hasta que haya completado el tamaño del archivo:
	do {

		//Levantar con lseek el cacho de archivo (CHUNK SIZE)
		lseek(fd->_fileno, written_bytes, SEEK_SET);
		printf("hago el seek del archivo");
		file_part->buffer_len = read(fd->_fileno, file_part->buffer_val, FILE_CHUNK_SIZE);

		printf("Logre leer %i bytes del archivo", &file_part->buffer_len);
		if (file_part->buffer_len == -1) {
			printf("read: (ERROR) El archivo %s no existe\n.", source_file);
			return 1;
		}

		result = write_file_1((filename)dest_file, *file_part, file_part->buffer_len, clnt);
		if (result == (int *) NULL) {
			clnt_perror (clnt, "write: (ERROR) Fallo la llamada al server\n");
			return 1;
		}

		printf("El server escribio: %i", &result);
		written_bytes += *result;

	}
	while (file_stat.st_size > written_bytes);

	//Cierro el archivo
	fclose(fd);

	return 0;
}



int main (int argc, char *argv[])
{
	if (argc < 5) {
		printf("usage: %s server_host -r|-w source_file dest_file\n", argv[0]);
		exit(1);
	}

	fs_prog_1(argv[1], argv[2][1], argv[3], argv[4]);

	exit(0);
}
