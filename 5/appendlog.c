#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAX_COMMANDS 5
#define RECORD_SIZE 32
#define NUM_RECORDS 1048576
#define FILE_SIZE sizeof(int) * 2 + NUM_RECORDS * RECORD_SIZE

void append_record(int fd, void *data);

int main(int argc, char *argv[]) {

    // Pairs of values (count, prefix), using two arrays
    char * prefix[MAX_COMMANDS];
    int count[MAX_COMMANDS];
    int counter = 0;
    for (int i=1; i<argc - 1; i+=2) {
        count[counter] = atoi(argv[i]);
        prefix[counter] = argv[i+1];
        counter++;
    }

    // Testing
    for (int i = 0; i < counter; i++) {
        printf("%d: %s\n", count[i], prefix[i]);
    }
    
    int fd;

    if ( (fd = open("log.dat", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1) {
        perror("open");
        return 1;
    }

    ftruncate(fd, FILE_SIZE);

    void * data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (data == NULL) {
        perror("mmap");
        return 2;
    }

    munmap(NULL, FILE_SIZE);

    close(fd);


}

void append_record(int fd, void *data);
