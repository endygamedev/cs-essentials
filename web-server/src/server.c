/* Name: Web Server implementation */
/* Author: Egor Bronnikov */
/* Last edited: 18-07-2022 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "server.h"


#define PORT 5555
#define BACKLOG 10
#define REQUEST "../example"


int main(void)
{
    int sockfd;
    struct sockaddr_in servaddr;
    
    memset(&servaddr, 0, sizeof(servaddr));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        fprintf(stderr, "Error: Can't open socket\n");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    int address_length = sizeof(servaddr);

    if (bind(sockfd, (struct sockaddr*)&servaddr, address_length) < 0) {
        fprintf(stderr, "Error: Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, BACKLOG) < 0) {
        fprintf(stderr, "Error: Listen failed\n");
        exit(EXIT_FAILURE);
    } else {
        fprintf(stdout, "Server is listening...\n\n");
    }
    
    launch(sockfd, servaddr, address_length);

    close(sockfd);
}


/**
 *  Listen connections for server
 *  @param sockfd           socket file descriptor
 *  @param servaddr         server address
 *  @param address_length   server address length
 */
void launch(int sockfd, struct sockaddr_in servaddr, int address_length)
{
    char buffer[BUFSIZ];
    long length;

    FILE *file_request = fopen(REQUEST, "r");
    FILE *logfile = fopen("/var/log/webserver.log", "w");

    if (!file_request) {
        fprintf(stderr, "Error: File can't be opened\n");
        exit(EXIT_FAILURE);
    }

    if (!logfile) {
        fprintf(stderr, "Error: Can't open logfile\n");
        exit(EXIT_FAILURE);
    }

    fseek(file_request, 0, SEEK_END);
    length = ftell(file_request);
    fseek(file_request, 0, SEEK_SET);

    char *http_request = malloc(length);

    if (http_request) {
        fread(http_request, 1, length, file_request);
    }

    fclose(file_request);
    
    while (1) {
        int accept_sockfd = accept(sockfd, (struct sockaddr*)&servaddr,
                                   (socklen_t *)&address_length);
        read(accept_sockfd, buffer, BUFSIZ);
        fprintf(logfile, "%s\n", buffer);
        write(accept_sockfd, http_request, strlen(http_request));
        close(accept_sockfd);
    }

    fclose(logfile);
}
