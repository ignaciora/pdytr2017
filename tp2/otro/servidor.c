#include "ej4bc.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

archivo *
leer_1_svc(nombre nombre_archivo, int offset, int cant_bytes,  struct svc_req *rqstp)
{
  static archivo result;

  int archivoFD = open(nombre_archivo, O_RDONLY);
  
  if(archivoFD == -1){
	printf("No se puede leer el archivo %s\n", nombre_archivo);
	result.archivo_len = -1;
	return &result;
  }
  lseek(archivoFD, offset, SEEK_SET);
  result.archivo_val = malloc(cant_bytes * sizeof(char));
  
  result.archivo_len = read(archivoFD, result.archivo_val, cant_bytes);
  printf("Leidos %d bytes del archivo %s:\n%s\n", result.archivo_len, nombre_archivo, result.archivo_val);
  
  close(archivoFD);
  
	return &result;
}

int *
escribir_1_svc(nombre nombre_archivo, archivo buffer, int cant_bytes,  struct svc_req *rqstp)
{
	static int  result;

	int archivoFD = open(nombre_archivo, O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
	
	if(archivoFD == -1){
	  archivoFD = creat(nombre_archivo, O_WRONLY);
	  fchmod(archivoFD, 0b111101101);
	}
	result = write(archivoFD, buffer.archivo_val, cant_bytes);
	
	printf("Se escribieron %d bytes\n", result);

	close(archivoFD);
	
	return &result;
}

int *
cantbytes_1_svc(nombre nombre_archivo,  struct svc_req *rqstp)
{
	printf("Calculando tamanio: ");
	static int  result;
	int archivoFD = open(nombre_archivo, O_RDONLY);
	struct stat sb;

	if (fstat(archivoFD, &sb) == -1) {
	  result = -1;
	  return &result;
	}
	printf("%lld - %d\n", sb.st_size, sb.st_size);
	
	result = sb.st_size;
		
	return &result;
}