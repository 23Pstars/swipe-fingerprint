#include <stdlib.h>
#include <printf.h>

#include "bmp.h"

/**
 * reverse order tiap n-pixel (max sampling 255 pixel)
 *
 * @param source
 * @param target
 * @param height
 * @param width
 * @param sampling_size
 */
void block_reverse(u_int8_t *source, u_int8_t *target, int32_t height, int32_t width, u_int8_t sampling_size) {

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

/**
 * potong array menjadi bagian yang diinginkan
 *
 * @param source, data utuh
 * @param height, tinggi total data utuh
 * @param width, lebar total data utuh
 * @param h_size, tinggi yang diinginkan untuk dipotong
 * @param w_size, lebar yang diinginkan untuk dipotong
 * @param h_offset, geser vertical
 * @param w_offset, geser horizontal
 * @return
 */
u_int8_t *
block_slice(u_int8_t *source, int32_t height, int32_t width, u_int16_t h_size, u_int16_t w_size, u_int16_t h_offset,
            u_int16_t w_offset) {

    u_int16_t i, j;
    u_int8_t *block = malloc(sizeof(u_int8_t) * h_size * w_size);

    for (i = 0; i < h_size; i++)

        for (j = 0; j < w_size; j++)

            block[(i * w_size) + j] = (u_int8_t) (h_offset + i > height - 1 || w_offset + j > width - 1 ? 0 : source[
                    ((i + h_offset) * width) + j + w_offset]);

    return block;
}