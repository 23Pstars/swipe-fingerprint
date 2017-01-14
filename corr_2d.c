//
// Created by Ahmad Zafrullah on 9/27/16.
//


#include <stdlib.h>
#include <math.h>

#include "corr_2d.h"

/* Sum of Absolute Differences */
int SAD_2d(unsigned char **source, unsigned char **target, int h_shift, int w_shift) {
    int i, j, sum = 0;
    for (i = 0; i < CORR_HEIGHT_SIZE; i++)
        for (j = 0; j < CORR_WIDTH_SIZE; j++)
            sum += abs((i + h_shift > CORR_HEIGHT_SIZE - 1 || j + w_shift > CORR_WIDTH_SIZE - 1 ?
                        0 : *(*(source + i + h_shift) + j + w_shift)) - *(*(target + i) + j));
    return sum;
}

/* Zero-mean Sum of Absolute Differences */
float ZSAD_2d(unsigned char **source, unsigned char **target, int h_shift, int w_shift) {
    int i, j;
    float sum = 0, source_mean = mean_2d(source), target_mean = mean_2d(target);
    for (i = 0; i < CORR_HEIGHT_SIZE; i++)
        for (j = 0; j < CORR_WIDTH_SIZE; j++)
            sum += fabs(((i + h_shift > CORR_HEIGHT_SIZE - 1 || j + w_shift > CORR_WIDTH_SIZE - 1 ?
                          0 : *(*(source + i + h_shift) + j + w_shift)) - source_mean) -
                        (*(*(target + i) + j) - target_mean));
    return sum;
}

/* Sum of Squared Differences */
int SSD_2d(unsigned char **source, unsigned char **target, int h_shift, int w_shift) {
    int i, j, sum = 0;
    for (i = 0; i < CORR_HEIGHT_SIZE; i++)
        for (j = 0; j < CORR_WIDTH_SIZE; j++)
            sum += pow(((i + h_shift > CORR_HEIGHT_SIZE - 1 || j + w_shift > CORR_WIDTH_SIZE - 1 ? 0 : *(
                    *(source + i + h_shift) + j + w_shift)) - *(*(target + i) + j)), 2);
    return sum;
}

/* Zero-mean Sum of Squared Differences */
float ZSSD_2d(unsigned char **source, unsigned char **target, int h_shift, int w_shift) {
    int i, j;
    float sum = 0, source_mean = mean_2d(source), target_mean = mean_2d(target);
    for (i = 0; i < CORR_HEIGHT_SIZE; i++)
        for (j = 0; j < CORR_WIDTH_SIZE; j++)
            sum += pow((((i + h_shift > CORR_HEIGHT_SIZE - 1 || j + w_shift > CORR_WIDTH_SIZE - 1 ?
                          0 : *(*(source + i + h_shift) + j + w_shift)) - source_mean) -
                        (*(*(target + i) + j) - target_mean)), 2);
    return sum;
}

/* Mean of Block array */
float mean_2d(unsigned char **block) {
    int i, j;
    float sum = 0;
    for (i = 0; i < CORR_HEIGHT_SIZE; i++)
        for (j = 0; j < CORR_WIDTH_SIZE; j++)
            sum += *(*(block + i) + j);
    return sum / (CORR_HEIGHT_SIZE * CORR_WIDTH_SIZE);
}