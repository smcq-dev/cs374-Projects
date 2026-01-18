#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    DIR *dir;
    struct dirent *structd;
    const char *path = ".";
    if (argc > 2) {
        fprintf(stderr, "usage: filescanner [directory]\n");
        exit(1);
    }

    if (argc == 1) {
       dir = opendir(path);
    }
    
    if (argc == 2) {
        dir = opendir(argv[1]);
    }

    if (dir == NULL) {
        perror("Opening the directory is causing:");
        exit(1);
    }

    while ((structd = readdir(dir)) != NULL) {
        printf("%s\n", structd->d_name);
    }
        

    closedir(dir);
}