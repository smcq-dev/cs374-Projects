#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define RECORD_SIZE 32
#define NUM_RECORDS 1048576
#define FILE_SIZE sizeof(int) * 2 + NUM_RECORDS * RECORD_SIZE

int main(int argc, char *argv[]) {

    int fd = open("log.dat", O_RDWR | O_CREAT | O_TRUNC, 0666);

    ftruncate(fd, FILE_SIZE);

    void * data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    munmap(NULL, FILE_SIZE);

    close(fd);


}
