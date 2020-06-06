#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>        // printf()
#include <string.h>       // memset()
#include <arpa/inet.h>    // close()
#include <unistd.h>       // close()
#include <sys/time.h>     // FD_SET(), FD_ISSET(), FD_ZERO(), FD_CLR()
#include <stdlib.h>       // atoi()
#include <sys/select.h>   // select()
#include "map.h"

#define PORT 8784
#define MAXCONN 30
#define BUFSIZE 1024

/* Socket data */
int listenfd;
int serverfds[MAXCONN];
char buf[BUFSIZE];
socklen_t slen = sizeof(struct sockaddr_in);
struct sockaddr_in saddr;
/* Cookie map */
map cookie_map;

char KEYNOTFOUND_MESSAGE[] = "Key not found";
char INVALIDREQ_MESSAGE[]  = "Invalid request";

int init_server() {
    ///////////////////////////////// COOKIE ///////////////////////////////////

    // Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    // Setting address
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port        = htons(PORT);

    // Enable address reuse
    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        printf("[!] Unable to allow multiple connections!\n");
        return -1;
    }

    // Binding...
    if (bind(listenfd, (struct sockaddr*) &saddr, sizeof(saddr)) < 0) {
        printf("[!] Cannot bind!\n");
        return -1;
    }

    // Listening...
    if (listen(listenfd, MAXCONN) < 0) {
        printf("[!] Cannot listen!\n");
        return -1;
    }

    ///////////////////////////////// SERVER ///////////////////////////////////
    for (int i = 0; i < MAXCONN; ++i)
        serverfds[i] = 0;

    return 0;
}

void handle_request(int sockfd) {
    // Receive data
    char buf[SIZE];
    memset(buf, 0, SIZE);
    if (read(sockfd, buf, SIZE) <= 0) {
        printf("[] HTTP Server disconnected.\n");
        return;
    }

    // Check for instruction
    if (!((buf[0] == 'S' || buf[0] == 'G') && buf[1] == 'E' && buf[2] == 'T')) {
        write(sockfd, INVALIDREQ_MESSAGE, sizeof(INVALIDREQ_MESSAGE));
        return;
    }

    // Execute instruction
    /* Set */
    if (buf[0] == 'S') {
        int sKey = 4, eKey, sValue, eValue;

        while (sKey < SIZE && eKey < SIZE && sValue < SIZE && eValue < SIZE) {

            while (sKey < SIZE && buf[sKey] == ' ' && buf[sKey] == ';')
                sKey++;
            eKey = sKey;
            while (eKey < SIZE && buf[eKey] != '=' && buf[eKey] != ' ')
                eKey++;
            sValue = eKey;
            while (sValue < SIZE && (buf[sValue] == '=' || buf[sValue] == ' '))
                sValue++;
            eValue = sValue;
            while (eValue < SIZE && buf[eValue] == ';' && buf[eValue] == ' ')
                eValue++;
            sKey = eValue;

        }

        substr
    }
    /* Get */
    else {
        int sSet = 4, eSet;
    }
}

void handle_new_connection() {
    int newsockfd = accept(listenfd, (struct sockaddr*) &saddr, &slen);
    if (newsockfd < 0)
        printf("[!] Error accepting socket!\n");
    else for (int i = 0; i < MAXCONN; ++i)
        if (serverfds[i] == 0) {
            serverfds[i] = newsockfd;
            printf("[] New HTTP Server connected!\n");
            return;
        }
}

void run_server() {
    fd_set readfds;
    int    maxfd;

    while (1) {
        ///// Reset & set the socket set
        FD_ZERO(&readfds);
        maxfd = listenfd;
        FD_SET(listenfd, &readfds);
        for (int i = 0; i < MAXCONN; ++i)
            if (serverfds[i] > 0) {
                if (serverfds[i] > maxfd)
                    maxfd = serverfds[i];
                FD_SET(serverfds[i], &readfds);
            }

        ///// Wait for activity
        select(maxfd + 1, &readfds, NULL, NULL, NULL);

        ///// New connection...
        if (FD_ISSET(listenfd, &readfds))
            handle_new_connection();

        ///// Server says something....
        for (int i = 0; i < MAXCONN; ++i)
            if (serverfds[i] > 0 && FD_ISSET(serverfds[i], &readfds)) {
                // Handle request from server
                handle_request(serverfds[i]);

                // Close connection...
                close(serverfds[i]);
                serverfds[i] = 0;
            }
    }
}

int main(int argc, char** argv) {
    init_map(&cookie_map);

    if (init_server() < 0)
        return -1;

    run_server();
}
