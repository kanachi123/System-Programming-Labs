#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src_path = argv[1];
    const char *dst_path = argv[2];

    int src_fd = open(src_path, O_RDONLY);
    if(src_fd < 0) {
        perror("open source");
        return EXIT_FAILURE;
    }

    struct stat st;
    if(fstat(src_fd, &st) < 0) {
        perror("fstat");
        close(src_fd);
        return EXIT_FAILURE;
    }

    size_t filesize = st.st_size;

    int dst_fd = open(dst_path, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if(dst_fd < 0) {
        perror("open destination");
        close(src_fd);
        return EXIT_FAILURE;
    }

    if(filesize == 0) {
        close(src_fd);
        close(dst_fd);
        return EXIT_SUCCESS;
    }

    if(ftruncate(dst_fd, filesize) < 0) {
        perror("ftruncate");
        close(src_fd);
        close(dst_fd);
        return EXIT_FAILURE;
    }

    void *src_map = mmap(NULL, filesize, PROT_READ, MAP_SHARED, src_fd, 0);
    if(src_map == MAP_FAILED) {
        perror("mmap source");
        close(src_fd);
        close(dst_fd);
        return EXIT_FAILURE;
    }

    void *dst_map = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dst_fd, 0);
    if(dst_map == MAP_FAILED) {
        perror("mmap destination");
        munmap(src_map, filesize);
        close(src_fd);
        close(dst_fd);
        return EXIT_FAILURE;
    }

    memcpy(dst_map, src_map, filesize);

    munmap(src_map, filesize);
    munmap(dst_map, filesize);
    close(src_fd);
    close(dst_fd);

    return EXIT_SUCCESS;
}
