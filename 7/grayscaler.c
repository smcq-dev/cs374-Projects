#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include "ppm.h"

struct slice_info {
    int r;
    int g;
    int b;
    int start_y;
    int slice_height;
    int img_width;
    int thread_num;
    struct ppm *img;
};

void *run(void *args) {
    struct slice_info *info = args;
    
    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;
    int pixel = 0;
    int gray = 0;
    int gray_pixel = 0;

    for (int i = 0; i < info->slice_height; i++) {

        for (int j = 0; j < info->img_width; j++) {
            pixel = ppm_get_pixel(info->img, j, info->start_y + i);

            int r = PPM_PIXEL_R(pixel);
            int g = PPM_PIXEL_G(pixel);
            int b = PPM_PIXEL_B(pixel);

            r_sum += r;
            g_sum += g;
            b_sum += b;

            gray = (299*r + 587*g + 114*b) / 1000;
            gray_pixel = PPM_PIXEL(gray, gray, gray);
            ppm_set_pixel(info->img, j, info->start_y + i, gray_pixel);          
        }
    }
    info->r = r_sum;
    info->g = g_sum;
    info->b = b_sum;

    printf("Thread %d: %d %d\n", info->thread_num, info->start_y, info->slice_height);
    return NULL;
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "usage: grayscaler num_threads ppm_image ppm_output_image\n");
        exit(1);
    }

    int num_threads = atoi(argv[1]);

    pthread_t thread[num_threads];

    struct slice_info slices[num_threads];

    struct ppm *img = ppm_read(argv[2]);

    int remainder = img->height % num_threads;
    int slice_height = (img->height - remainder) / num_threads;
    
    int y_pos = 0;

    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;

    for (int i = 0; i < num_threads; i++) {
        
        if (i == num_threads - 1) {
            slice_height = slice_height + remainder;
        }

        slices[i].img_width = img->width;
        slices[i].slice_height = slice_height;
        slices[i].start_y = y_pos;
        slices[i].thread_num = i;
        slices[i].img = img;
        pthread_create(thread + i, NULL, run, &slices[i]);

        y_pos += slice_height;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(thread[i], NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        r_sum += slices[i].r;
        g_sum += slices[i].g;
        b_sum += slices[i].b;    
    }

    int r_avg = r_sum / (img->width * img->height);
    int g_avg = g_sum / (img->width * img->height);
    int b_avg = b_sum / (img->width * img->height);

    printf("Average R: %d\nAverage G: %d\nAverage B: %d\n", r_avg, g_avg, b_avg);

    if (ppm_write(img, argv[3]) == -1) {
        puts("Error writing ppm.");
    }

    ppm_free(img);
}