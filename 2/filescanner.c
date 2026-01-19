#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX_SIZE 1024

int scan_directories(char *path);

int main(int argc, char *argv[]) {
    char *path = (argc == 2) ? argv[1] : ".";

    if (argc > 2) {
        fprintf(stderr, "usage: filescanner [directory]\n");
        exit(1);
    }

    scan_directories(path);
    return 0;
}


int scan_directories(char *path) {
    struct dirent *structd;
    struct stat statbuf;
    int total_size = 0;
    char fullpath[PATH_MAX_SIZE];

    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((structd = readdir(dir)) != NULL) {

        if ((strcmp(structd->d_name,".") == 0) || (strcmp(structd->d_name,"..") == 0)) {
            continue;
        }

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, structd->d_name);
        lstat(fullpath, &statbuf);

        if (S_ISREG(statbuf.st_mode)) {
            total_size += statbuf.st_size;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            total_size += scan_directories(fullpath);
        }
        
    }

    closedir(dir);
    printf("%s: %d\n", path, total_size);
    return total_size;
}