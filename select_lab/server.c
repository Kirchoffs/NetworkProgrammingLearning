#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE 4096
#define EXTRA_LINE 4

int main(int argc, char **argv) {
    int listen_fd, connect_fd;
    socklen_t client_len;
    struct sockaddr_in client_addr, server_addr;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(12345);

    bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    /* backlog of listen: 1024 */
    listen(listen_fd, 1024);

    client_len = sizeof(client_addr);
    connect_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
    char buf[MAX_LINE];
    for (;;) {
        if (read(connect_fd, buf, MAX_LINE) == 0) {
            printf("client quit");
            break;
        }
        printf("receive: %s\n", buf);

        char send_line[MAX_LINE + EXTRA_LINE];
        sprintf(send_line, "Hi, %s", buf);

        int nbytes = sizeof(send_line);

        if (write(connect_fd, send_line, nbytes) != nbytes)
            error(1, errno, "write error\n");
    }

    close(listen_fd);
    close(connect_fd);

    exit(0);
}