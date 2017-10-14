#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


void error(char *msg)
{
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[])
{
    int sockfd, portno, n, i, m, mtotal, buffer_size;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int32_t ret;
    mtotal = 0;


    //validacion de parametros
    if (argc < 4) {
       fprintf(stderr, "usage %s hostname port buffer_size\n", argv[0]);
       exit(0);
    }


    //declaracion e inicializacion del buffer
    buffer_size = atoi(argv[3]);
    //char *buffer;
    //buffer = (char *) malloc(buffer_size);
    char buffer[buffer_size];

    for(i = 0 ; i < buffer_size ; i++) {
       buffer[i] = 'A';
    }


    //creacion de sockets
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


    printf("enviando mensaje de %i bytes...\n", buffer_size);
    m = write(sockfd, buffer, strlen(buffer));

    //continua enviando los datos que faltan
    n = read(sockfd, &ret, sizeof(ret));
    mtotal += ntohl(ret);
    while (mtotal < buffer_size) {
         printf(">> bytes enviados = %i, faltan enviar = %i ...\n"
		, mtotal, &buffer[buffer_size] - &buffer[mtotal]);
         m = write(sockfd, &buffer[mtotal+1], buffer_size - mtotal);
         n = read(sockfd, &ret, sizeof(ret));
         if (n < 0) error("ERROR reading from socket");
         mtotal += ntohl(ret);
    }

    printf("mensaje enviado.\n");
    if (n < 0) error("ERROR writing to socket");
    printf("cerrando programa.\n");
    return 0;
}
