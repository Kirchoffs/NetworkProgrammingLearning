#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <unistd.h>

#define LISTENQ 12345
#define MAX_LINE 2048

int main(int argc, char **argv) {
    if (argc != 2) {
        error(1, 0, "usage: unixstreamserver <local_path>");
    }

    int listen_fd, connect_fd;
    socklen_t client_len;
    struct sockaddr_un client_addr, server_addr;

    listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        error(1, errno, "socket create failed");
    }

    char *local_path = argv[1];
    unlink(local_path);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, local_path);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error(1, errno, "bind failed");
    }

    if (listen(listen_fd, LISTENQ) < 0) {
        error(1, errno, "listen failed");
    }

    client_len = sizeof(client_addr);
    if ((connect_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
        if (errno == EINTR)
            error(1, errno, "accept failed");        /* back to for() */
        else
            error(1, errno, "accept failed");
    }

    char buf[MAX_LINE];

    while (1) {
        memset(buf, 0, sizeof(buf));
        if (read(connect_fd, buf, MAX_LINE) == 0) {
            printf("client quit");
            break;
        }
        printf("Receive: %s", buf);

        char send_line[MAX_LINE + 5];
        sprintf(send_line, "Hi, %s", buf);

        int nbytes = sizeof(send_line);

        if (write(connect_fd, send_line, nbytes) != nbytes)
            error(1, errno, "write error");
    }

    close(listen_fd);
    close(connect_fd);

    exit(0);
}