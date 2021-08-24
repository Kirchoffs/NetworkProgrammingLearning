#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

// The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
// The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
// The ntohl() function converts the unsigned integer netlong from network byte order to host byte order.
// The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.

/* read size bytes from socket fd */
size_t readn(int fd, void *buffer, size_t size) {
    char *buffer_pointer = buffer;
    int length = size;

    while (length > 0) {
        int result = read(fd, buffer_pointer, length);

        if (result < 0) {
            if (errno == EINTR)
                continue;
            else
                return (-1);
        } else if (result == 0)
            break;                /* EOF(End of File) */

        length -= result;
        buffer_pointer += result;
    }
    return (size - length);        /* return the actual number of bytes */
}

void read_data(int sockfd) {
    ssize_t n;
    char buf[1024];

    static int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        if ((n = readn(sockfd, buf, 1024)) == 0)
            return;

        time += n;
        fprintf(stdout, "read %d bytes \n", time);
        sleep(1);
    }
}

int main(int argc, char **argv) {
    int listen_fd, connect_fd;
    socklen_t client_len;
    struct sockaddr_in client_addr, server_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(12345);

    bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    /* backlog of listen: 1024 */
    listen(listen_fd, 1024);

    for (;;) {
        client_len = sizeof(client_addr);
        connect_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
        read_data(connect_fd);
        close(connect_fd);          /* close connect socket，instead of listen socket */
    }
}