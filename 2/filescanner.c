#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    DIR *dir;
    struct dirent *structd;
    struct stat statbuf;
    const char *path = (argc == 2) ? argv[1] : ".";

    if (argc > 2) {
        fprintf(stderr, "usage: filescanner [directory]\n");
        exit(1);
    }

    dir = opendir(path);

    if (dir == NULL) {
        perror("Opening the directory is causing:");
        exit(1);
    }

    int directory_size = 0;
    char fullpath[1024];

    while ((structd = readdir(dir)) != NULL) {
        printf("%s\n", structd->d_name);
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, structd->d_name);
        lstat(fullpath, &statbuf);

        if (S_ISREG(statbuf.st_mode)) {
            directory_size += statbuf.st_size;
        }
        
    }

    snprintf(fullpath, sizeof(fullpath), ": %d", directory_size);
    printf("%s: %d\n", path, directory_size);
        

    closedir(dir);
}


//char * search_directories(void){}