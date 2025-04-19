#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void error(char * msg){
    perror(msg);
    exit(1);
}

int main(){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; /* Define host computer on the internet - netdb.h*/

    printf("Starting the execution\n");
    portno = 8080;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[256];

    if(sockfd < 0){
        error("Failed to create socket");
    }

    server = gethostbyname("localhost"); /* Take name and return a pointer to hostent struct*/
    if(server == NULL){
        fprintf(stderr, "No server found");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)server->h_addr_list[0],
            (char *)&serv_addr.sin_addr,
            server->h_length
    ); /* Copy bytes from h_addr_list to sin_addr*/

    serv_addr.sin_port = htons(portno);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in)) < 0){
        error("Connection failed");
    }

    printf("\nConnected!\n");
    close(sockfd);
}