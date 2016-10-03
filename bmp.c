#include <stdlib.h>

#include "bmp.h"

/* reverse order tiap n-pixel (max sampling 255 pixel) */
void reverse_order(u_int8_t *source, u_int8_t *target, int32_t height, int32_t width, u_int8_t sampling_size) {

    u_int16_t i, j, k, rate = (u_int16_t) height / sampling_size;

    /* segments */
    for (i = 0; i < rate; i++)

        /* width in sampling (block) */
        for (j = 0; j < sampling_size; j++)

            /* element (column) of rows */
            for (k = 0; k < width; k++)

                target[(i * sampling_size * width) + (j * width) + k] = source[
                        ((height - (sampling_size * (i + 1))) * width) + (j * width) + k];

}