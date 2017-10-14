#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


void error(char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, n, m, ntotal, buffer_size;
     struct sockaddr_in serv_addr, cli_addr;     
     int32_t conv;
     ntotal = 0;


     //validacion de parametros
     if (argc < 3) {
         fprintf(stderr, "usage %s port buffer_size\n", argv[0]);
         exit(1);
     }


     //declaracion del buffer
     buffer_size = atoi(argv[2]);
     //char *buffer;
     //buffer = (char *) malloc(buffer_size);
     char buffer[buffer_size];


     //creacion de sockets
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");


     bzero(buffer,buffer_size);
     printf("esperando mensaje del cliente...\n");
     n = read(newsockfd, buffer, buffer_size);
     if (n < 0) error("ERROR reading from socket");
     ntotal += n;

     //continua pidiendo los datos que falten     
     printf(">> %i bytes recibidos...\n", ntotal);
     while (ntotal < buffer_size) {
          conv = htonl(ntotal);
          m = write(newsockfd, &conv, sizeof(conv));
          n = read(newsockfd, &buffer[ntotal+1], buffer_size - ntotal);
          if (n < 0) error("ERROR reading from socket");
          ntotal += n;
          printf(">> %i bytes recibidos...\n", ntotal);
     }
     //aviso al cliente de que termino
     conv = htonl(ntotal);
     m = write(newsockfd, &conv, sizeof(conv));

     printf("mensaje de %i bytes recibido.\n", ntotal);
     printf("cerrando programa.\n");
     return 0; 
}
