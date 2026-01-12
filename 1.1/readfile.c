#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
int main(int argc, char *argv[]) {

    char* file = argv[1];
    char buffer[2048];

    int fd  = open(file,O_RDONLY);
    if(fd == -1) {
        perror("Open for Read Only");
        return 1;
    }

    int n_bytes = read(fd, buffer, sizeof(buffer) - 1);

    write(1, buffer, n_bytes);

    write(1, "\n", 1);
    close(fd);


}