## Building

Command line:

* `make` to build. An executable called `pc` will be produced.
* `make clean` to clean up all build products except the executable.

VS Code:

* The default build task runs `make`.

## Files

* `pc.c`: The main code of the program.
* `Makefile`: Code to build the executable with make. 
* `eventbuf.c`: Code with the eventbuf functions 
* `eventbuf.h`: Header file for eventbuf.c

## Data

This program uses quite a few data structures. The main ones worth mentioning are the fifo queue we use to add and process events. It's source code is in eventbuf.c and eventbuf.h. Besides this we use simple data structures and semaphores to run our program. 

## Functions

* `reserve_seat()`:
    * `pthread_mutex_lock()`: Lock a designated mutex.
    * `pthread_mutex_unlock()`: Unlock a designated mutex.
* `free_seat()`: 
    * `pthread_mutex_lock()`: Lock a designated mutex.
    * `pthread_mutex_unlock()`: Unlock a designated mutex.
* `verify_seat_count()`:
    * `pthread_mutex_lock()`: Lock a designated mutex.
    * `pthread_mutex_unlock()`: Unlock a designated mutex.
* `*seat_broker()`: 
    *  `rand()`: Generates a random number in a desired range. 
    * `reserve_seat()`: Reserve seat by changing array value to 1.
    * `free_seat()`: Open seat up by changing array value to 0.
    * `verify_seat_count()`: Checks if the total number of seats is equal to the taken amount of seats. 
* `main()`: 
    * `calloc()`: Allocates multiple blocks of memory to an array and initializes to 0.
    * `pthread_create()`: Create threads
    * `pthread_join()`: Join to wait for threads to finish.
    * `free()`: Free associated memory

* `run_producer()`:
    * `sem_wait()`: Wait on sem_spaces to block if queue is full.
    * `sem_wait()`: Wait on sem_mutex to lock the queue.
    * `eventbuf_add()`: Add the event to the shared queue.
    * `printf()`: Print the production message inside the mutex lock.
    * `sem_post()`: Unlock sem_mutex.
    * `sem_post()`: Signal sem_items that a new event is available.
* `run_consumer()`:
    * `sem_wait()`: Wait on sem_items to block if queue is empty.
    * `sem_wait()`: Wait on sem_mutex to lock the queue.
    * `eventbuf_empty()`: Check if queue is empty to detect shutdown.
    * `eventbuf_get()`: Remove an event from the shared queue.
    * `printf()`: Print the consumption message inside the mutex lock.
    * `sem_post()`: Unlock sem_mutex.
    * `sem_post()`: Signal sem_spaces that a slot has freed up.
* `main()`:
    * `atoi`(): Parse the four command line arguments into integers.
    * `eventbuf_create()`: Allocate and initialize the shared event queue.
    * `sem_open_temp()`: Create sem_mutex, sem_items, and sem_spaces.
    * `malloc()`: Allocate each thread's ID argument before passing it.
    * `pthread_create()`: Spawn all producer and consumer threads.
    * `pthread_join()`: Wait for all producer threads to finish.
    * `sem_post()`: Wake each consumer by posting to sem_items once per consumer.
    * `pthread_join()`: Wait for all consumer threads to finish.
    * `eventbuf_free()`: Free the shared event queue.
    * `sem_close()`: Close sem_mutex, sem_items, and sem_spaces.
* `sem_open_temp`():
    * `sem_open()`: Create and open a named semaphore with an initial value.
    * `sem_unlink()`: Unlink the semaphore so it cleans up when the process exits.
    * `sem_close()`: Close the semaphore if unlinking fails.

## Notes
* Print order could probably be deterministic, so add that if you want but for this program it's fine being not comnpletly deterministic. 
