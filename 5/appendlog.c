#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define MAX_COMMANDS 5
#define RECORD_SIZE 32
#define NUM_RECORDS 1048576
#define FILE_SIZE sizeof(int) * 2 + NUM_RECORDS * RECORD_SIZE

struct flock header_lock = {
    .l_type=F_WRLCK, // Or F_RDLCK for a read lock
    .l_whence=SEEK_SET,
    .l_start=0,
    .l_len= sizeof(int) * 2,
};

struct flock lock = {
    .l_type=F_WRLCK, // Or F_RDLCK for a read lock
    .l_whence=SEEK_SET,
    .l_start=0,
    .l_len=0,
};

struct flock unlock = {
    .l_type=F_UNLCK,  
    .l_whence=SEEK_SET,
    .l_start=0,
    .l_len=0,
};

void append_record(int fd, void *data, int count, char *prefix);

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

    int num_pairs = counter;

    // Opening logic
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

    // Forking and dumping logic

    int *offset = data;

    int *records = offset + 1;

    *offset = 0;
    *records = 0;

    pid_t pid;

    for (int i=0; i < num_pairs; i++) {
        pid = fork();
    
        if (pid == 0) {

            for (int j=0; j < count[i]; j++) {
                append_record(fd, data, j, prefix[i]);
            }
            exit(0);
        }
    
    }

    for (int i=0; i < num_pairs; i++) {
        wait(NULL);
    }

    fcntl(fd, F_SETLKW, &lock);

    int num_records = *records;

    for (int i=0; i < num_records; i++) {
        char *record = (char *)data + sizeof(int) * 2 + i * RECORD_SIZE;
        printf("%d: %s\n", i, record);
    }

    fcntl(fd, F_SETLKW, &unlock);

    // Closing logic

    munmap(NULL, FILE_SIZE);

    close(fd);

}

void append_record(int fd, void *data, int count, char *prefix) {
    
    fcntl(fd, F_SETLKW, &header_lock);

    int *offset = data;

    int *records = offset + 1;

    (*records) ++;

    int loc = *offset;

    *offset += RECORD_SIZE;

    fcntl(fd, F_SETLKW, &unlock);

    struct flock record_lock = {
    .l_type=F_WRLCK, // Or F_RDLCK for a read lock
    .l_whence=SEEK_SET,
    .l_start= 8 + loc,
    .l_len= RECORD_SIZE,
    };

    char *new_loc = (char *)data + 8 + loc;
    
    fcntl(fd, F_SETLKW, &record_lock);

    sprintf(new_loc, "%s %d", prefix, count);

    fcntl(fd, F_SETLKW, &unlock);

}
