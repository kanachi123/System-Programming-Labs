#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "unix_stream.h"

#define SV_SOCK_PATH "/tmp/echo.sock"
#define BUF_SIZE 256

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // Создаем сокет
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    // Подключаемся к серверу
    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    printf("Client: Connected to server.\n");

    // Чтение данных из stdin и отправка их в сокет
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        printf("Client: Sending %ld bytes to server: %.*s\n", (long) numRead, (int) numRead, buf);
        if (write(sfd, buf, numRead) != numRead) {
            perror("partial/failed write");
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    // Чтение ответа от сервера
    ssize_t numBytes = read(sfd, buf, BUF_SIZE);
    if (numBytes > 0) {
        printf("Client: Received response from server: %.*s\n", (int)numBytes, buf);
    } else if (numBytes == 0) {
        printf("Client: Server closed the connection.\n");
    } else {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    // Закрытие сокета
    close(sfd);
    return 0;
}
