//
// Created by Ahmad Zafrullah on 10/3/16.
//

#include <stdlib.h>
#include <math.h>

#include "correlation_algorithm.h"

/* Sum of Absolute Differences */
u_int16_t SAD(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    u_int8_t i, j;
    u_int16_t sum = 0;

    for (i = 0; i < height; i++)

        for (j = 0; j < width; j++)

            sum += abs((i + h_shift > height - 1 || j + w_shift > width - 1 ? 0 : source[
                    ((i + h_shift) * width) + j + w_shift]) - target[(i * width) + j]);

    return sum;
}

/* Zero-mean Sum of Absolute Differences */
float ZSAD(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    u_int8_t i, j;
    float sum = 0, source_mean = mean(source, height * width), target_mean = mean(target, height * width);

    for (i = 0; i < height; i++)

        for (j = 0; j < width; j++)

            sum += fabs(((i + h_shift > height - 1 || j + w_shift > width - 1 ? 0 : source[((i + h_shift) * width) + j +
                                                                                           w_shift]) - source_mean) -
                        (target[(i * width) + j] - target_mean));

    return sum;
}

/* Mean of Block array */
float mean(u_int8_t *block, u_int16_t length) {
    u_int8_t i;
    float sum = 0;
    for (i = 0; i < length; i++)
        sum += block[i];
    return sum / length;
}