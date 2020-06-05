#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>        // printf()
#include <string.h>       // memset()
#include <arpa/inet.h>    // close()
#include <unistd.h>       // close()
#include <pthread.h>      // pthread_create(), pthread_join()
#include <errno.h>
#include <sys/time.h>     // FD_SET(), FD_ISSET(), FD_ZERO(), FD_CLR()
#include <stdlib.h>       // atoi()
#include <sys/select.h>   // select()
#include <termios.h>      // ---()
#include <fcntl.h>        // fcntl()
#include "map.h"

#define PORT 8784
#define MAXCONN 30
#define BUFSIZE 1024

/* Socket data */
int port;
int listenfd;
int serverfds[MAXCONN];
char buf[BUFSIZE];
/* Cookie map */
map cookie_map;

char http_request_header[] = {
    "HTTP/2.0 200 OK\n\rContent-Type: text/html\n\r"
};

int setup() {
    ///////////////////////////////// COOKIE ///////////////////////////////////

    // Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    // Setting address
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family      = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port        = htons(port);

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

void close_connection(int i, int forced=0) {
    // If not forced, close willingly
    if (!forced) {
        shutdown(serverfds[i], SHUT_RDWR);
        char c; while(read(serverfds[i], &c, 1) >= 0);
    }

    // Close socket
    close(serverfds[i]);
    serverfds[i] = 0;
}

int header_match(char* buf) {
    int p = 0;
    for (; p < strlen(http_request_header); ++p)
        if (http_request_header[p] != buf[p])
            return -1;
    return p;
}

void handle_request(int server_no, char* buf, int beg) {
    if
}

void receive_from_server(int server_no) {
    // Abruptedly exit
    if (read(serverfds[server_no], buf, BUFSIZE) <= 0) {
        printf("[!] Server abruptedly exit. Doesn't know why...\n");
        close_connection(server_no, 1);
        return;
    }

    // Check string header...
    int p;
    if ((p = header_match(buf)) == -1) {
        printf("[!] Invalid request header!]\n");
        close_connection(server_no);
    }

    // Handle request from server
    handle_request(server_no, buf, p);

    // Properly exit
    close_connection(server_no);
}

void handle_new_connection() {
    socklen_t slen = sizeof(struct sockaddr_in);
    struct sockaddr_in saddr;

    int newsockfd = accept(listenfd, (struct sockaddr*) &saddr, &slen);
    if (newsockfd < 0)
        printf("[!] Error accepting socket!\n");
    else for (int i = 0; i < MAXCONN; ++i)
        if (serverfds[i] == 0)
            serverfds[i] = newsockfd;
}

void run_cookie_server() {
    fd_set readfds;
    int    maxfd;

    while (true) {
        // Reset & set the socket set
        FD_ZERO(&readfds);
        maxfd = listenfd;
        FD_SET(listenfd, &readfds);
        for (int i = 0; i < MAXCONN; ++i)
            if (serverfds[i] > 0) {
                if (serverfds[i] > maxfd)
                    maxfd = serverfds[i];
                FD_SET(serverfds[i], &readfds);
            }

        // Wait for activity
        select(maxfd + 1, &readfds, NULL, NULL, NULL);

        // New connection...
        if (FD_ISSET(listenfd, &readfds))
            handle_new_connection();

        // Server says something....
        for (int i = 0; i < MAXCONN; ++i)
            if (FD_ISSET(serverfds[i], &readfds))
                receive_from_server(i);
    }
}

int main(int argc, char** argv) {
    if (setup() < 0)
        return -1;

    run_cookie_server();
}
