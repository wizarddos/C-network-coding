#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 8080

void error(char * message){
    perror(message);
    exit(1);
}

int main(){
    struct sockaddr_in server;
    struct hostent *host;

    printf("Starting client");

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[256];

    if(sockfd < 0){
        error("Failed to create a socket");
    }

    host = gethostbyname("localhost");
    if(host == NULL)
        error("Failed to resolve hostname");
    
    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;

    memcpy( (char *) &server.sin_addr, (char *) host->h_addr_list[0], host->h_length);
    server.sin_port = htons(PORT);

    socklen_t len = sizeof(server);
    for(;;){
        char *mess = "PING!";
        int n = sendto(sockfd, mess, strlen(mess), 0, (struct sockaddr *) &server, len);
        if(n < 0)
            error("Failed to send PING");
        
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &server, &len);
        if(n < 0)
            error("Failed to read PONG!");
        
        printf("%s\n", buffer);
        bzero(buffer, sizeof(buffer));
    }

    close(sockfd);
}
