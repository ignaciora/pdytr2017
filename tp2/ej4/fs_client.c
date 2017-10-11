#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef FILE_CHUNK_SIZE
	#define FILE_CHUNK_SIZE 256
#endif


void fs_prog_1(char *host, char opt, char *filename)
{
	CLIENT *clnt;
	FILE *file;
	int result = 0;

	clnt = clnt_create(host, FS_PROG, FS_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	switch (opt) {
		/* read */
		case 'r':
			file = fopen(filename, "r+b"); //'r+b'?
			if (file == NULL) {
				clnt_perror(clnt, "read: (ERROR) Fallo la lectura del archivo local\n");
			}

			result = fs_read(filename, file, clnt);
			fclose(file);

			if (result == 0) {
				clnt_perror(clnt, "read: (ERROR) Fallo la lectura total o parcial del archivo que se encuentra en el servidor\n");
			}
			break;

		/* write */
		case 'w':
			file = fopen(filename, "rb"); //'rb'?
			if (file == NULL) {
				clnt_perror(clnt, "write: (ERROR) Fallo la lectura del archivo local\n");
			}

			result = fs_write(filename, file, clnt);
			fclose(file);

			if (result == 0) {
				clnt_perror(clnt, "write: (ERROR) Fallo la escritura total o parcial del archivo en el servidor\n");
			}
			break;

		default:
			break;
	}

	clnt_destroy(clnt);
}


/* Devuelve 1 si se logro extraer del server el archivo completo y '0' en caso contrario. */
int fs_read(char *filename, FILE *file, CLIENT *clnt) {
	enum clnt_stat retval;
	read_ret result;
	read_args  args;

	retval = read_1(&args, &result, clnt);
	if (retval != RPC_SUCCESS) {
		clnt_perror (clnt, "read: (ERROR)\n");
	}

	return 0;
}


/* Devuelve '1' si se logro depositar en el server el archivo completo y '0' en caso contrario. */
int fs_write(char *filename, FILE *file, CLIENT *clnt) {
	enum clnt_stat retval;
	long result = 0;
	write_args  args;
	struct stat file_stat;
	//int remaining_bytes = 0;

	/* args setup */
	//args = malloc(sizeof(args));
	//args.buffer = malloc(sizeof(buffer));
	args.filename = filename;
	args.buffer.buffer_len = 0;
	args.buffer.buffer_val = malloc(FILE_CHUNK_SIZE);

	/* file stats */
	if (fstat64(file->_fileno, &file_stat) != 0) {
		clnt_perror(clnt, "write: (ERROR) Fallo la lectura del archivo\n");
		return 0;

		//otra forma de hallar el file size:
		//bzero(args.buffer, sizeof(char));
		//fseeko(file, 0, SEEK_END);
		//args.bytes = ftello(file);
		//rewind(file);
	}

	if (file_stat.st_size == 0) {
		clnt_perror(clnt, "write: (ERROR) Archivo vacio\n");
		return 0;
	}


	/* DEBUG */
	printf("filename: %s\n", args.filename);
	printf("file total size: %li\n", file_stat.st_size);
	printf("comenzando loop...\n\n");
	int loop_counter = 0; //BORRAR

	
	do {
		if (fseeko(file, args.buffer.buffer_len, SEEK_CUR) == -1) {
			clnt_perror(clnt, "write: (ERROR) No se logro establecer la siguiente posicion en el archivo\n");
			break;
		}


		/* ESTO DEBERIA IR EN LUGAR DE LO DE ABAJO, EL PROBLEMA ES QUE LA RESTA DA MAL

		remaining_bytes = file_stat.st_size - args.buffer.buffer_len;
		if (remaining_bytes > FILE_CHUNK_SIZE) {
			args.buffer.buffer_val = realloc(args.buffer.buffer_val, FILE_CHUNK_SIZE);
			args.bytes = read(file->_fileno, &args.buffer.buffer_val, FILE_CHUNK_SIZE);
		}
		else {
			args.buffer.buffer_val = realloc(args.buffer.buffer_val, 43);
			args.bytes = read(file->_fileno, &args.buffer.buffer_val, 43);
		}
		*/

		//no chequea el caso en que sea menos que el chunk
		args.buffer.buffer_val = realloc(args.buffer.buffer_val, FILE_CHUNK_SIZE);

		//no chequea el caso en que queden menos bytes que el chunk
		args.bytes = read(file->_fileno, &args.buffer.buffer_val, FILE_CHUNK_SIZE);
	

		/* DEBUG */
		printf("loop_counter: %d\n", ++loop_counter);
		//printf("remaining_bytes: %i\n", remaining_bytes);
		printf("bytes: %li\n", args.bytes);
		printf("buffer_len: %d\n", args.buffer.buffer_len);
		printf("buffer_val: %s\n", &args.buffer.buffer_val);
		printf("buffer_val size: %li\n", sizeof(args.buffer.buffer_val));
		printf("llamando al server...\n\n");
	

		if (args.bytes == 0) { //VERIFICAR ERROR EN EL ULTIMO CICLO
			clnt_perror(clnt, "write: (ERROR) No se logro leer el archivo completo antes de enviarlo al servidor\n");
			break;
		}



		/* DEBUG - TEST DATA */
		/*
		printf("TEST DATA\n");
		write_args *args_test;
		buffer* sub_buffer;
		//args_test = malloc(sizeof(write_args));
		//sub_buffer = malloc(sizeof(buffer));
		sub_buffer->buffer_val = malloc(43);
		long bytes_test = read(file->_fileno, sub_buffer->buffer_val, 43);
		printf("bytes: %li\n", bytes_test);
		printf("buffer_len: %d\n", sub_buffer->buffer_len);
		printf("buffer_val: %s\n", sub_buffer->buffer_val);
		printf("buffer_val size: %li\n", sizeof(sub_buffer->buffer_val));
		printf("llamando al server...\n\n");
		*/



		/* server call */
		retval = write_1(&args, &result, clnt);
		if (retval != RPC_SUCCESS) {
			clnt_perror(clnt, "write: (ERROR) Fallo la llamada a la funcion WRITE\n");
			break;
		}

		args.buffer.buffer_len += result;
	}
	while (args.buffer.buffer_len < file_stat.st_size);

	free(args.buffer.buffer_val);

	return (args.buffer.buffer_len == file_stat.st_size) ? 1 : 0;
}


int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("usage: %s server_host -r|-w file\n", argv[0]);
		exit(1);
	}

	fs_prog_1(argv[1], argv[2][1], argv[3]);

	exit(0);
}
