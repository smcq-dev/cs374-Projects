#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    char *path = (argc == 2) ? argv[1] : ".";

    int pfds[2];
    pipe(pfds);

    pid_t pid = fork();

    int stdin = pfds[0];
    int stdout = pfds[1];

    if (pid == 0) {
       dup2(pfds[0], 0);
       close(pfds[1]);
       execlp("wc", "wc", "-l", NULL);

    }
    else {

        dup2(pfds[1], 1);
        close(pfds[0]);
        execlp("ls", "ls", "-1a", path, NULL);
        
    }

}