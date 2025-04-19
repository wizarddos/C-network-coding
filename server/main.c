#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

struct sockaddr_in serv_addr, cli_addr; /*Declare 2 structures to hold server address and additional one*/

void error(char * mess){
    perror(mess);
    exit(1);
}

int main(){

    printf("Executing code\n");
    /*initialize socket
        Params:
            Host - AF_INET means what type of address can our socket communicate with
            Type of connection - SOCK_STREAM for TCP and SOCK_DGRAM for UDP
            Protocol - 0 is IP

        Returns -1 if socket was not created
    */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sockfd < 0){
        error("Error while creating a socket");
    }

    char buffer[256];
    bzero((char *) &serv_addr, sizeof(serv_addr)); /*Initialize buffer to 0*/
    int portno = 8080;

    serv_addr.sin_family = AF_INET; /* Should always be set like this */  
    serv_addr.sin_port = htons(8080); /*Convert INT from Host byte order to network byte order*/
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) /*Binding - Allocating a port number to a socket*/
        error("Error while binding");

    listen(sockfd, 5); /*Listen for connections*/
    /*
        1. Socket
        2. Max Backlog queue - Number of connections that can wait to be handled (5 is mostly max)
    */

    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); /*Create a new socket that extracts the first connection in queue*/
    if(newsockfd < 0)
        error("Error while accepting");


    bzero(buffer, 256);
    int n;
    n = write(newsockfd, "Connection Established!", 24);
    if(n < 0){
        error("Error while writing");
    }

    n = read(newsockfd, buffer, 255);
    if(n < 0){
        error("Error while reading: ");
        printf("%s\n", buffer);
    }
    
    
    close(newsockfd);
    close(sockfd);
    return 0;
}