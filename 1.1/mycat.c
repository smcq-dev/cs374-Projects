#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char buffer[2048];
    if (argc == 1) {
        while (1){
            int bytes = read(0, buffer, sizeof(buffer) - 1);
            if (bytes == 0) {
                return 0;
            }
            else if (bytes < 0) {
                perror("Error: stdin is 0");
                return 0;
            }
            write(1, buffer, bytes);
        }
    }

    else {
        for (int i = 1; i < argc; i++) {
            char* file = argv[i];

            int fd  = open(file,O_RDONLY);
            if(fd == -1) {
                perror("Open for Read Only");
                return 1;
            }
            while (1){
                int n_bytes = read(fd, buffer, sizeof(buffer) - 1);
                if (n_bytes == 0) {
                    break;
                }
                else if (n_bytes < 0) {
                    perror("Error: stdin is 0");
                    break;
                }
                write(1, buffer, n_bytes);
            }

            close(fd);
        }   
    }


}