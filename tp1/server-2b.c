/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 100000

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, n;
     struct sockaddr_in serv_addr, cli_addr;     

     //char *buffer;
     //buffer = (char *) malloc(BUFFER_SIZE);
     char buffer[BUFFER_SIZE];
     
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

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

     bzero(buffer,BUFFER_SIZE);

     printf("esperando mensaje del cliente...\n");

     n = read(newsockfd,buffer,BUFFER_SIZE - 1);
     if (n < 0) error("ERROR reading from socket");

     printf("mensaje recibido: \n%s\n",buffer);
     printf("ultimo caracter recibido (debe ser X): %c\n", buffer[BUFFER_SIZE - 2]);

     printf("enviando confirmacion al cliente...\n");
     n = write(newsockfd,"OK",2);
     if (n < 0) error("ERROR writing to socket");

     printf("cerrando programa.\n");
     return 0; 
}
