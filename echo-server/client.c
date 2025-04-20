#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


void error(char * message){
    perror(message);
    exit(1);
}

int main(){
    struct sockaddr_in server;
    struct hostent *internet_host;

    printf("Client starting\n\n");
    int portn = 9090;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[256];

    if(sockfd < 0){
        error("Failed to create a socket");
    }

    internet_host = gethostbyname("localhost");
    if(internet_host == NULL){
        error("No server found");
    }

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;

    memcpy((char *) internet_host->h_addr_list[0], (char *) &server.sin_addr, internet_host->h_length);

    server.sin_port = htons(portn);

    if(connect(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) < 0){
        error("Connection failed\n");
    }

    printf("Connection established!\n");
    int n = read(sockfd, buffer, 255);

    char banner[256];
    memcpy(&banner, &buffer, 255);
    printf("%s", banner);

    for(int i = 0; i < 10; i++){
        printf("Your Message: ");
        scanf("%s", buffer);

        if(write(sockfd, buffer, 255) < 0){
            error("Failed to send the message");
        }

        printf("\n");
        read(sockfd, buffer, 255);
        printf("%s\n", buffer);
    }

    close(sockfd);
}