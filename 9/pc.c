#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "eventbuf.h"

sem_t *sem_mutex, *sem_items, *sem_spaces;

struct eventbuf *eb;
volatile int done = 0;
int n_producers, n_consumers, events_produced, events_outstanding;

sem_t *sem_open_temp(const char *name, int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void *run_producer(void *args) {
    int id = *(int *)args;
    free(args);

    for (int i = 0; i < events_produced; i++) {
        int event = id * 100 + i;
        sem_wait(sem_spaces);
        sem_wait(sem_mutex);

        eventbuf_add(eb, event);
        printf("P%d: adding event %d\n", id, event);

        sem_post(sem_mutex);
        sem_post(sem_items);
    }

    printf("P%d: exiting\n", id);
    return NULL;
}

void *run_consumer(void *args) {
    int id = *(int *)args;
    free(args);

    while (1) {
        sem_wait(sem_items);
        sem_wait(sem_mutex);

        if (done && eventbuf_empty(eb)) {
            sem_post(sem_mutex);
            sem_post(sem_spaces);
            break;
        }

        int event = eventbuf_get(eb);
        printf("C%d: got event %d\n", id, event);

        sem_post(sem_mutex);
        sem_post(sem_spaces);
    }

    printf("C%d: exiting\n", id);
    return NULL;

}

int main(int argc, char *argv[]) {
    (void)argc;
    n_producers = atoi(argv[1]);
    n_consumers = atoi(argv[2]);
    events_produced = atoi(argv[3]);
    events_outstanding = atoi(argv[4]);

    eb = eventbuf_create();

    pthread_t producers[n_producers];
    pthread_t consumers[n_consumers];

    sem_mutex = sem_open_temp("sem_mutex", 1);
    sem_items = sem_open_temp("sem_items", 0);
    sem_spaces = sem_open_temp("sem_spaces", events_outstanding);

    for (int i = 0; i < n_producers; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(producers + i, NULL, run_producer, (void *)arg);
    }

    for (int i = 0; i < n_consumers; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(consumers + i, NULL, run_consumer, (void *)arg);
    }

    for (int i = 0; i < n_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    done = 1;
    for(int i = 0; i < n_consumers; i++) {
        sem_post(sem_items);
    }

    for (int i = 0; i < n_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    eventbuf_free(eb);
    sem_close(sem_mutex);
    sem_close(sem_items);
    sem_close(sem_spaces);

}