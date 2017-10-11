#include "ej4bc.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BLOQUE
#define BLOQUE 200
#endif

int main (int argc, char *argv[])
{
	char *host;
	CLIENT *clnt;
	archivo* arch;
	archivo* archivito;
	nombre nombre_archivo;
	int cant_lecturas;
	int* cant_escritos;
	int result;
	int* cant_bytes;

	if (argc < 2) {
		printf ("Uso: %s host nombre_archivo\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	clnt = clnt_create (host, EJ4, EJ4_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	
	nombre_archivo = argv[2];
	cant_lecturas = 0;
	arch = malloc(sizeof(archivo));
	arch->archivo_len = 0;
	arch->archivo_val = malloc(BLOQUE);
	
// 	cant_bytes = cantbytes_1(nombre_archivo, clnt);
// 	printf("Cantidad de bytes de %s:\t%d\n", nombre_archivo, *cant_bytes);
	
	do{
	  archivito = leer_1(nombre_archivo, cant_lecturas * BLOQUE, BLOQUE, clnt);
	  if(archivito->archivo_len == -1){
		printf("El archivo %s no existe\n.", nombre_archivo);
		exit(1);
	  }
	  arch->archivo_val = realloc(arch->archivo_val, cant_lecturas * BLOQUE * sizeof(char) + sizeof(char) * archivito->archivo_len + sizeof(char));
	  arch->archivo_len += archivito->archivo_len;
	  strcpy(arch->archivo_val + cant_lecturas * BLOQUE, archivito->archivo_val);
	  cant_lecturas++;
	}while( archivito->archivo_len == BLOQUE);
	
	printf("El archivo contiene %d bytes:\n%s\n", arch->archivo_len, arch->archivo_val);
	
	printf("Guardando el archivo localmente...");
	
	int archivoFD = open(nombre_archivo, O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
	
	if(archivoFD == -1){
	  archivoFD = creat(nombre_archivo, O_WRONLY);
	  fchmod(archivoFD, 0b111101101);
	}
	result = write(archivoFD, arch->archivo_val, arch->archivo_len);
	
	printf("Hecho (se escribieron %d bytes)\n", result);

	close(archivoFD);
	
	strcat(nombre_archivo, "_copia");
	printf("Escribiendo la copia en el servidor...  ");
	cant_escritos = escribir_1(nombre_archivo, *arch, arch->archivo_len, clnt);
	printf("Escritos %d bytes\n", *cant_escritos);
	
	clnt_destroy (clnt);
	exit (0);
}
