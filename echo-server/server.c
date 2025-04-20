#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

struct sockaddr_in server, cli_addr;

void error(char * message){
    perror(message);
    exit(1);
}

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        error("Failed to create a socket");
    }

    bzero((char *) &server, sizeof(server));
    int port = 9090;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0){
        error("Failed to bind a socket");
    }
    
    listen(sockfd, 5);
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd < 0){
        error("Failed to accept\n");
    }

    char buffer[256];
    int n = write(newsockfd, "Echo server running!\n", 21);

    if(n < 0){
        error("Failed to write to the socket");
    }

    for (;;){
        n = read(newsockfd, buffer, 255);
        if( n < 0){
            error("Failed to read echo info");
        }

        if(n == 0){
            printf("Client Disconnected");
            break;
        }
        printf("%s\n", buffer);
        if(write(newsockfd, buffer, 255) < 0){
            error("Failed to return echo");
        }
    }

    close(newsockfd);
    close(sockfd);
}