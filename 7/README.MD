## Building

Command line:

* `make` to build. An executable called `grayscaler` will be produced.
* `make clean` to clean up all build products except the executable.
* `make pristine` to clean up all build products entirely.

VS Code:

* The default build task runs `make`.

## Files

* `grayscaler.c`: The main logic related to creating the gray ppm image and calculating average RGB values.
* `ppm.c`: Implements ppm functions defined in ppm.h
* `ppm.h`: Code defining PPM_PIXEL macros and the ppm struct. Defines functions for ppm.c

## Data

There is a ppm image file we use that has 3 byte pixels. We access it's data through the ppm struct. We also use a list of structs that define info relevant to what each thread needs to know, including the original ppm struct, the starting y position, the slice height etc. We keep a list of these structs to pass into each thread. 

## Functions

* `main()`
  * `ppm_read()`: Reads a PPM image from a file into a struct
  * `pthread_create()`: Creates a thread for each image slice
    * `run()`: Thread function, converts a horizontal slice of the image to grayscale and accumulates RGB sums
      * `ppm_get_pixel()`: Reads a pixel from the image at a given x,y position
      * `ppm_set_pixel()`: Sets a pixel from the image at a given x,y position
  * `pthread_join()`: Waits for all threds to finish
  * `ppm_write()`: Writes the modified image to a new file
  * `ppm_free()`: Frees the image memory

## Notes

* We could add more functionality that allows us to pick from a list of filters including grayscale. 