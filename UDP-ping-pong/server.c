#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080



void error(char * message){
    perror(message);
    exit(1);
}

int main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[256];
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    if(sockfd < 0){
        error("Failed to initialize socket");
    }

    if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0){
        error("Failed to bind a server");
    }

    socklen_t len = sizeof(server);
    for(;;){
        struct sockaddr_in client;
        socklen_t clilen = sizeof(client);

        bzero(&buffer, sizeof(buffer));
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,  (struct sockaddr * )&client, &clilen);
        if(n < 0)
            error("Failed to recieve");
        
        if(!strcmp(buffer, "PING!")){
            char * pong = "PONG!";
            printf("%s\n", pong);
            n = sendto(sockfd, pong, strlen(pong), 0, (struct sockaddr * )  &client, clilen);
            if(n < 0)
                error("Failed to send PONG");
        }
        
    }

    close(sockfd);
}