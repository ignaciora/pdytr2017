#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFFER_SIZE 100000

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, i;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    //char *buffer;
    //buffer = (char *) malloc(BUFFER_SIZE);
    char buffer[BUFFER_SIZE];

    //init buffer
    for(i = 0 ; i < BUFFER_SIZE ; i++) {
       buffer[i] = 'A';
    }
    //marca de fin
    buffer[BUFFER_SIZE - 2] = 'X';

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //printf("Please enter the message: ");
    //bzero(buffer,BUFFER_SIZE);
    //fgets(buffer,BUFFER_SIZE - 1,stdin);

    printf("enviando mensaje...\n");
    n = write(sockfd,buffer,strlen(buffer));

    printf("mensaje enviado.\n");

    if (n < 0) 
         error("ERROR writing to socket");

    printf("limpiando buffer...\n");

    bzero(buffer,BUFFER_SIZE);

    printf("esperando respuesta del server...\n");

    n = read(sockfd,buffer,BUFFER_SIZE - 1);
    if (n < 0) 
         error("ERROR reading from socket");

    printf("respuesta recibida: %s\n", buffer);
    printf("cerrando programa.\n");
    return 0;
}
