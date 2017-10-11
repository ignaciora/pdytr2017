#include "ej4bc.h"

int main (int argc, char *argv[])
{
	char *host;
	CLIENT *clnt;
	archivo* arch;
	int offset, cantidad;
	
	clnt = clnt_create ("localhost", EJ4, EJ4_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror ("localhost");
		exit (1);
	}
	
	offset = atoi(argv[2]);
	cantidad = atoi(argv[3]);
	
	printf("Leyendo %d bytes desde la posicion del %dº byte del archivo %s:\n", cantidad, offset, argv[1]);
	arch = leer_1(argv[1], offset, cantidad, clnt);
	
	printf("Se leyo:\n%s\n", arch->archivo_val);
	
}