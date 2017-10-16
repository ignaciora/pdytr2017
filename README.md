service rpcbind status
    -> si no esta corriendo usar: rpcbind -i -w
    -> si no esta instalado usar: sudo apt-get install rpcbind

Compilar los archivos usando: make -f Makefile.fs


fs_client (cliente):

usage: %s [--help] server_host (-r|-w|--copy) source_file dest_file [--test-timeout]

     --help		muestra esta ayuda
server_host		localhost o una IP
         -r		read, traer un archivo del server
         -w		write, enviar un archivo al server
     --copy		trae un archivo del server y lo vuelve a enviar generando una copia
source_file		ruta del archivo local o nombre del archivo del server (origen)
  dest_file		ruta del archivo local o nombre del archivo del server (destino)

El servidor se ejecuta sin argumentos
