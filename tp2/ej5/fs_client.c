#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>

#ifndef FILE_CHUNK_SIZE
	#define FILE_CHUNK_SIZE 255
#endif


/* Toma un instante de tiempo segun el reloj de la computadora y lo retorna en segundos. */
double take_time()
{
	double sec;
	struct timeval tv;
  
	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}


/* Devuelve 0 si se logro extraer del server el archivo completo y 1 si se lo extrajo en forma parcial o si no se recupero nada. */
int fs_read(char *source_file, char *dest_file, CLIENT *clnt)
{
	FILE *file;
	buffer *file_part;
	int reads_count = 0;
	double timetick;

	file = fopen(dest_file, "wb");
	if (file == NULL) {
		printf("read: (ERROR) El archivo %s no existe\n", source_file);
		return 1;
	}

	timetick = take_time(); //COMIENZA A TOMAR EL TIEMPO QUE TARDA LA COMUNICACION CON EL SERVIDOR

	do {
		file_part = read_file_1((filename)source_file, reads_count * FILE_CHUNK_SIZE, FILE_CHUNK_SIZE, clnt);
		if (file_part == (buffer *) NULL) {
			clnt_perror (clnt, "read: (ERROR) Fallo la llamada al servidor\n");

			printf("EJ5-A (loop): tiempo de la comunicacion en segundos: %f\n", take_time() - timetick);
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
	double timetick;

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

	timetick = take_time(); //COMIENZA A TOMAR EL TIEMPO QUE TARDA LA COMUNICACION CON EL SERVIDOR

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

	printf("EJ5-A: tiempo de la comunicacion en segundos: @%f\n", take_time() - timetick);

	fclose(file);
	free(file_part);
	return 0;
}


void fs_prog_1(char *host, char opt, char *source_file, char *dest_file, int timeout_always)
{
	CLIENT *clnt;
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	clnt = clnt_create(host, FS_PROG, FS_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	if (timeout_always) {
		clnt_control(clnt, CLSET_TIMEOUT, (char *) &tv);
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


void show_help(char *program_name)
{
	printf("usage: %s [--help] server_host (-r|-w) source_file dest_file [--test-timeout]\n\n", program_name);
	printf("--help			muestra esta ayuda\n");
	printf("server_host		localhost o una IP\n");
	printf("-r			read, traer un archivo del server (ej 5-a timeout)\n");
	printf("-w			write, enviar un archivo al server (ej 5-a promedio)\n");
	printf("source_file		ruta del archivo local o nombre del archivo del server (origen)\n");
	printf("dest_file		ruta del archivo local o nombre del archivo del server (destino)\n");
	printf("--test-timeout		siempre tirar timeout, pedido en el ej 5-c\n\n");
}


int main (int argc, char *argv[])
{
	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		show_help(argv[0]);
		exit(0);
	}

	if (argc < 5) {
		printf("usage: %s [--help] server_host (-r|-w) source_file dest_file [--test-timeout]\n", argv[0]);
		exit(1);
	}

	if (argc == 6 && strcmp(argv[5], "--test-timeout") == 0) {
		fs_prog_1(argv[1], argv[2][1], argv[3], argv[4], 1);
		exit(0);
	}

	fs_prog_1(argv[1], argv[2][1], argv[3], argv[4], 0);

	exit(0);
}
