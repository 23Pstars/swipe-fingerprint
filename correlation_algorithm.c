//
// Created by Ahmad Zafrullah on 10/3/16.
//

#include <stdlib.h>
#include <math.h>

#include "correlation_algorithm.h"

/**
 * Sum of Absolute Differences
 *
 * @param source, block yang akan dicari korelasinya
 * @param target, block template yang menjadi pembanding, tinggi max adalah shift (-) dan (+) dari tinggi source
 * @param height, tinggi block dari source
 * @param width, lebar block dari source atau target
 * @param h_shift, geser vertical
 * @param w_shift, geser horizontal
 * @return
 */
u_int32_t SAD(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    int16_t i, j;
    u_int32_t sum = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {

            /**
             * jika ada over-index pada horizontal
             * set nilai = 0
             */
//            if (i + h_shift < 0 || i + h_shift > height - 1 || j + w_shift < 0 || j + w_shift > width - 1)
//                sum += target[(i * width) + j];
//                sum += 0;
//            else
//                sum += abs(source[((i + h_shift) * width) + (j + w_shift)] - target[(i * width) + j]);

            /**
             * versi 2
             * shift vertical tetap mengambil dari memory sebelumnya,
             * jadi yang dihitung hanya shift horizontal saja
             */
            if (j + w_shift < 0 || j + w_shift > width - 1)
//                sum += 0;
                sum += source[(i * width) + j];
            else
                sum += abs(source[(i * width) + j] - target[((i + h_shift) * width) + (j + w_shift)]);

        }
    }

    return sum;
}

/**
 * Sum of Squared Differences
 *
 * @param source
 * @param target
 * @param height
 * @param width
 * @param h_shift
 * @param w_shift
 * @return
 */
u_int32_t SSD(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    int16_t i, j;
    u_int32_t sum = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {

            if (j + w_shift < 0 || j + w_shift > width - 1)
                sum += 0;
//                sum += abs((source[(i * width) + j]) << 2);
            else
                sum += abs((source[(i * width) + j] - target[((i + h_shift) * width) + (j + w_shift)]) << 2);

        }
    }

    return sum;
}

/**
 * Zero-mean Sum of Absolute Difference
 *
 * @param source
 * @param target
 * @param height
 * @param width
 * @param h_shift
 * @param w_shift
 * @return
 */
float ZSAD(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    u_int8_t i, j;
    u_int16_t length = height * width;
    float sum = 0, source_mean = mean(source, length, 0), target_mean = mean(target, length, h_shift * width);

    for (i = 0; i < height; i++) {

        for (j = 0; j < width; j++) {

            if (j + w_shift < 0 || j + w_shift > width - 1)
//                sum += 0;
                sum += source[(i * width) + j];
            else
                sum += fabs(source[(i * width) + j] - source_mean - target[((i + h_shift) * width) + (j + w_shift)] +
                            target_mean);

        }
    }

    return sum;
}

float ZSSD(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    u_int8_t i, j;
    u_int16_t length = height * width;
    float sum = 0, source_mean = mean(source, length, 0), target_mean = mean(target, length, h_shift * width);

    for (i = 0; i < height; i++) {

        for (j = 0; j < width; j++) {

            if (j + w_shift < 0 || j + w_shift > width - 1)
//                sum += 0;
                sum += powf(source[(i * width) + j], 2);
            else
                sum += powf((source[(i * width) + j] - source_mean - target[((i + h_shift) * width) + (j + w_shift)] +
                             target_mean), 2);

        }
    }

    return sum;
}

/* Mean of Block array */
float mean(u_int8_t *block, u_int16_t length, u_int16_t shift) {
    u_int16_t i;
    float sum = 0;
    for (i = 0; i < length; i++)
        sum += *(block + i + shift);
    return sum / length;
}