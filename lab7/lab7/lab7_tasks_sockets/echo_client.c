#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "unix_stream.h"  // BUF_SIZE и SV_SOCK_PATH

void *recv_thread(void *arg) {
    int sfd = *(int *)arg;
    char buf[BUF_SIZE];
    ssize_t n;

    while ((n = read(sfd, buf, BUF_SIZE)) > 0) {
        write(STDOUT_FILENO, buf, n);
    }

    return NULL;
}

int main() {
    int sfd;
    struct sockaddr_un addr;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, recv_thread, &sfd);

    // write->stdin->server->clinets
    while (fgets(buf, BUF_SIZE, stdin) != NULL) {
        write(sfd, buf, strlen(buf));
    }

    close(sfd);
    return 0;
}
