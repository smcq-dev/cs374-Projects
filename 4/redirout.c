#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: redirout filename command [arg1 [arg2 ... ] ]\n");
        exit(1);
    }

    int fd;
    if ((fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY)) == -1) {
        perror("Open failure");
    }

    dup2(fd, 1);

    execvp(argv[2], &argv[2]);

}