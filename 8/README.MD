## Building

Command line:

* `make` to build. An executable called `clife` will be produced.
* `make clean` to clean up all build products except the executable.

VS Code:

* The default build task runs `make`.

## Files

* `reservations.c`: The main code of the program.
* `Makefile`: Code to build the executable with make. 

## Data

This program does not use a lot of data structures. It mainly uses one array that we use to store if a seat is taken or not. We set the seats corresponding index to 0 if it is open, and 1 if it is taken. 

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



## Notes

* This program gets very slow when dealing with larger amounts of brokers, seats and transactions. 