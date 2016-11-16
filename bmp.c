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
 * merge suatu block (source) kedalam suatu data (target)
 *
 * @param source, block yang akan di-merge
 * @param target, data utuh tujuan merge
 * @param height, tinggi dari block yang akan di-merge (source)
 * @param width, lebar dari block yang akan di-merge (source)
 * @param offset, offset untuk tiap block target didalam target
 * @param h_shift, pergeseran (vertical) source dari target
 * @param w_shift, pergeseran (horizontal) source dari target
 * @param default_w_value, default value untuk pergeseran horizontal
 */
void block_merge(u_int8_t *source, u_int8_t *target, u_int8_t height, u_int8_t width, int16_t offset, int16_t h_shift,
                 int16_t w_shift, u_int8_t default_w_value) {

    u_int16_t i, j;

//    printf("\n\noffset: %d, h_shift: %d, w_shift: %d\n", offset, h_shift, w_shift);

    /**
     * looping untuk setiap row setinggi height dari source
     */
    for (i = 0; i < height; i++) {

        /**
         * looping untuk setiap cell sepanjang width dari source
         */
        for (j = 0; j < width; j++) {

            if (j + w_shift < 0) {                          /** geser kiri */
//                printf("h;%d w;%d %d:%d\t\t", i, j, *(target + ((offset + i + h_shift + 1) * width) + j + w_shift),
//                       default_w_value);
//                target[((offset + i + h_shift + 1) * width) + j + w_shift] = default_w_value;
                *(target + ((offset + i + h_shift + 1) * width) + j + w_shift) = default_w_value;
            } else if (j + w_shift > width - 1) {           /** geser kanan */
//                printf("h;%d w;%d %d:%d\t\t", i, j, *(target + ((offset + i + h_shift - 1) * width) + j + w_shift),
//                       default_w_value);
//                target[((offset + i + h_shift - 1) * width) + j + w_shift] = default_w_value;
                *(target + ((offset + i + h_shift - 1) * width) + j + w_shift) = default_w_value;
            } else {
//                printf("h:%d w:%d %d:%d\t\t", i, j,
//                       *(target + ((offset + i + h_shift) * width) + j + w_shift), *(source + (i * width) + j));
//                target[((offset + i + h_shift) * width) + j + w_shift] = source[(i * width) + j];
                *(target + ((offset + i + h_shift) * width) + j + w_shift) = *(source + (i * width) + j);
            }
        }
//        printf("\n");
    }

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