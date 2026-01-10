#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SV_SOCK_PATH "/tmp/echo.sock"  // Путь к Unix-сокету
#define BUF_SIZE 256                   // Размер буфера для обмена данными

int main() {
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // Создаем серверный сокет
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    printf("Server: Socket created successfully.\n");

    // Удаляем старый сокет, если он существует
    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        perror("remove error");
        exit(EXIT_FAILURE);
    }

    // Привязываем сокет к пути
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    printf("Server: Socket bound to %s\n", SV_SOCK_PATH);

    // Начинаем прослушивать сокет
    if (listen(sfd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    printf("Server: Listening for connections...\n");

    // Основной цикл сервера
    for (;;) {
        printf("Server: Waiting for connections...\n");
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            perror("accept error");
            continue; // Пропускаем ошибку и ждем следующего подключения
        }
        printf("Server: Connection accepted.\n");

        // Чтение данных от клиента и отправка их обратно (эхо)
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            printf("Server: Received %ld bytes from client: %.*s\n", (long) numRead, (int) numRead, buf);
            if (write(cfd, buf, numRead) != numRead) {
                perror("partial/failed write");
                exit(EXIT_FAILURE);
            }
            printf("Server: Sent back %ld bytes: %.*s\n", (long) numRead, (int) numRead, buf);
        }

./unix_stream_client        if (numRead == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
        }

        printf("Server: Closing client connection.\n");
        if (close(cfd) == -1) {
            perror("close error");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
