#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define COMMAND_LINE_MAX_SIZE 2048

int main(int argc, char *argv[]) {

    char buffer[COMMAND_LINE_MAX_SIZE];
    char *token;

    while(1) {
        printf("mush$ ");
        
        fgets(buffer, sizeof(buffer), stdin);

        if ((token = strtok(buffer, " ")) != NULL) do {
        printf("%s\n", token);
  
        } while ((token = strtok(NULL, " ")) != NULL);

       
    }
}
