#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int make_socket(uint16_t port) {
    int sock;
    struct sockaddr_in name;

    /* create IPV4 socket. */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    /* bind port and IP address */
    name.sin_family = AF_INET; /* IPv4 */
    name.sin_port = htons (port);  /* port */
    name.sin_addr.s_addr = htonl (INADDR_ANY); /* wildcard address */
    /* transform IPv4 address to general address, and pass the length of it */
    if (bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return sock;
}