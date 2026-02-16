#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <math.h>

#define MAX_INPUT_SIZE 128
#define MAX_VELOCITY pow(2, 31) - 1

volatile sig_atomic_t sigusr1_happened;
volatile sig_atomic_t sigusr2_happened;

void sigusr1_handler(int sig) {
    (void)sig;
    sigusr1_happened = 1;
}

void sigusr2_handler(int sig) {
    (void)sig;
    sigusr2_happened = 1;
}


int main(void) {

    int speed = 0;

    char buf[MAX_INPUT_SIZE];

    printf("PID=%d\n", getpid());

    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    struct sigaction sa1 = {
        .sa_handler = sigusr1_handler,
        .sa_flags = SA_RESTART,
    };
    sigemptyset(&sa1.sa_mask);

    if (sigaction(SIGUSR1, &sa1, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    struct sigaction sa2 = {
        .sa_handler = sigusr2_handler,
        .sa_flags = SA_RESTART,
    };
    sigemptyset(&sa2.sa_mask);

    if (sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    sigset_t zero;
    sigemptyset(&zero);

    while(1) {
        if (speed < 0) {
            speed = 0;
            printf("oops! sorry but speed can't be negative, speed: 0\n");
        }
        if (speed > MAX_VELOCITY) {
            speed = MAX_VELOCITY;
            printf("your going a little fast there bud, slow down, speed: %d\n", speed);
        }
        
        sigusr1_happened = 0;
        sigusr2_happened = 0;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);

        int result = pselect(1, &readfds, NULL, NULL, NULL, &zero);
        if(result == -1 && errno == EINTR) {
            if (sigusr1_happened == 1) {
                speed -= 1;
                printf("decrease speed to: %d\n", speed);
            }
            if (sigusr2_happened == 1) {
                speed += 1;
                printf("increase speed to: %d\n", speed);
            }
        }
        if (result > 0){
            fgets(buf, sizeof(buf), stdin);
            
            for(int i=0; i < (int)strlen(buf); i++) {
                if (buf[i] == 'q') {
                    exit(0);
                }
                if (buf[i] == '+') {
                    speed += 1;
                    printf("increase speed to: %d\n", speed);
                }
                if (buf[i] == '-') {
                    speed -= 1;
                    printf("decrease speed to: %d\n", speed);
                }
            }
        }
    }

}