#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define COMMAND_LINE_MAX_SIZE 2048
#define MAX_WORDS_COMMAND_LINE 128

int main(int argc, char *argv[]) {

    char buffer[COMMAND_LINE_MAX_SIZE];
    char *token;
    char *words[MAX_WORDS_COMMAND_LINE];
   

    while(1) {
        int counter = 0;
        printf("mush$ ");
        
        fgets(buffer, sizeof(buffer), stdin);

        if ((token = strtok(buffer, " \n")) != NULL) do {
        printf("%s\n", token);
        words[counter] = token;
        counter ++;
  
        } while ((token = strtok(NULL, " \n")) != NULL);

        words[counter] = NULL;
        for (int i = 0; words[i] != NULL; i++) {
            printf("%s\n", words[i]);
        }

    }
}
