/**
 * source untuk daftar algoritma correlation
 * berbasis array (pointer) 1 dimensi (vektor)
 */

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <printf.h>

#include "config.h"
#include "corr.h"
#include "bmp.h"

/**
 * Reconstruct the overlapping image
 *
 * @param pixel_data
 * @param pixel_data_generated
 * @param height
 * @param width
 */
void reconstruct(u_int8_t *pixel_image, u_int8_t *pixel_image_generate, int32_t height, int32_t width) {

    /**
     * simpan sementara block hasil slice
     */
    u_int8_t *block_source, *block_target;

    u_int32_t __min_diff = 66585600,    /// perbedaan terkecil dari tiap block (max 255^2 x 128 x 8 = 66585600)
            __min_diff_tmp = 0;         /// perbedaan sementara sebagai pembanding dengan __min_diff

    u_int32_t __image_size = (u_int32_t) (height * width);

//    float __min_diff_f = 0, __min_diff_tmp_f = 0;

    int8_t j, k;                        /// iterasi shift horizontal dan vertical

    int16_t i,                          /// untuk keperluan iterasi saja (range -32768 : 32767)
            __min_h_diff,               /// jarak vertical dengan perbedaan terkecil
            __min_w_diff,               /// jarak horizontal dengan perbedaan terkecil

            __block_size = (u_int16_t) (SAMPLING_SIZE * width),         /// jumlah byte dalam 1 block
            __rate = (u_int16_t) (__image_size / __block_size);         /// jumlah keseluruhan block (step)

    int16_t __rate_h_offset = SAMPLING_SIZE;        /// offset h setelah block di-merge

//    for (int16_t x = 0; x < 32 * infoheader.width; x++)
//        printf("%s%s%d\t", (x % __block_size == 0 ? "\n" : ""),
//               (x % infoheader.width == 0 ? "\n" : ""), pixel_image[x]);
//
//    printf("\n\n\n");

//    u_int8_t *block = block_slice(pixel_image, infoheader.height, infoheader.width, SAMPLING_SIZE,
//                                  (u_int16_t) infoheader.width, 1, 0);
//    for (j = 0; j < __block_size; j++)
//        printf("%s%s%d\t", (j % (infoheader.width * SAMPLING_SIZE) == 0 ? "\n" : ""),
//               (j % infoheader.width == 0 ? "\n" : ""), *(block + j));
//
//    free(block);
//
//    for (i = 0; i < __rate; i++) {
//        printf("h_offset: %d, w_offset: %d\n", (i * SAMPLING_SIZE), 0);
//    }
//
//    return -1;

    /**
     * copy 1 block awal ke pixel_image_generate
     */
    memcpy(pixel_image_generate, pixel_image, __block_size);

    /**
     * Mulai iterasi untuk mencocokkan
     * tiap block pixel dengan block selanjutnya
     *
     * < __rate - 1 karena target langsung menggunakan 2 block
     * untuk dijadikan template
     */
    for (i = 0; i < __rate - 1; i++) {

        /**
         * berisi block yang akan dicocokan dengan block template,
         * ukurannya SAMPLING x width
         * w_shift + 1 --> berada di ujung template
         */
        block_source = block_slice(pixel_image, height, width, SAMPLING_SIZE, (u_int16_t) width,
                                   (u_int16_t) ((i + 1) * SAMPLING_SIZE), 0);

        /**
         * berisi template untuk pembanding, 2 x block source
         * ukuran 2 x SAMPLING x width
         */
        block_target = block_slice(pixel_image, height, width, 2 * SAMPLING_SIZE, (u_int16_t) width,
                                   (u_int16_t) (i * SAMPLING_SIZE), 0);

//        u_int8_t *block1 = block_slice(pixel_image, infoheader.height, infoheader.width, SAMPLING_SIZE,
//                                       3, (u_int16_t) (i * SAMPLING_SIZE), 0),
//                *block2 = block_slice(pixel_image, infoheader.height, infoheader.width, SAMPLING_SIZE,
//                                      3, (u_int16_t) ((i + 1) * SAMPLING_SIZE), 0);

//        if (i == 0) {
//            for (u_int16_t x = 0; x < __block_size; x++)
//                printf("%s%s%d,\t", (x % __block_size == 0 ? "\n" : ""),
//                       (x % infoheader.width == 0 ? "\n" : ""), *(block_source + x));
//            printf("\n");
//            for (u_int16_t x = 0; x < 2 * __block_size; x++)
//                printf("%s%s%d,\t", (x % __block_size == 0 ? "\n" : ""),
//                       (x % infoheader.width == 0 ? "\n" : ""), *(block_target + x));
//            printf("\n");
//        }
//        for (u_int16_t x = 0; x < 3 * SAMPLING_SIZE; x++)
//            printf("%s%s%d\t", (x % 3 == 0 ? "\n" : ""),
//                   (x % 3 == 0 ? "\n" : ""), *(block1 + x));
//        printf("\n\n\n");
//        for (u_int16_t x = 0; x < 3 * SAMPLING_SIZE; x++)
//            printf("%s%s%d\t", (x % 3 == 0 ? "\n" : ""),
//                   (x % 3 == 0 ? "\n" : ""), *(block2 + x));
//        printf("\n\n\n");

        /**
         * reset jarak terkecil horizontal dan vertical
         */
        __min_h_diff = -HEIGHT_RANGE_OFFSET;
        __min_w_diff = -WIDTH_RANGE_OFFSET;

        /**
         * Lakukan pencocokan secara vertical dan horizontal
         * vertical mulai dari -HEIGHT_RANGE_OFFSET s/d 0
         * horizontal mulai dari -WIDTH_RANGE_OFFSET s/d WIDTH_RANGE_OFFSET
         *
         * khusus block pertama, shift vertical ke atas (-) tidak berlaku
         * karena memory nya kosong
         */
        for (j = (int8_t) __min_h_diff; j <= 0; j++) {
            for (k = (int8_t) __min_w_diff; k <= WIDTH_RANGE_OFFSET; k++) {

                /**
                 * hindari mencocokan dengan block yang sama
                 * atau dirinya sendiri
                 */
                if (j == 0 && k == 0)
                    continue;

                /**
                 * jika terdapat perbedaan yang lebih kecil
                 * diantara semua perbandingan vertical dan horizontal
                 *
                 * Gunakan algoritma korelasi yang ada:
                 * - SAD (best minimum)
                 * - ZSAD (best minimum)
                 * - SSD (best minimum)
                 * - ZSSD (best minimum)
                 */
                __min_diff_tmp = (u_int32_t) SAD(block_source, block_target, SAMPLING_SIZE, (u_int16_t) width,
                                                 (int8_t) (j + SAMPLING_SIZE), (int8_t) k);

                /**
                 * - NCC (best maximum)
                 * - ZNNC (best maximum)
                 */
//                __min_diff_tmp_f = ZNCC(block_source, block_target, SAMPLING_SIZE, (u_int16_t) width,
//                                        (int8_t) (j + SAMPLING_SIZE), (int8_t) k);

                /**
                 * @todo
                 * bagaimana jika memang tidak ada korelasi dari kedua block tersebut?
                 * (DIFF_THRESHOLD_MAX)
                 *
                 * jika memenuhi treshold lanjutkan
                 * ke __rate selanjutnya
                 */
                if (__min_diff_tmp <= DIFF_THRESHOLD_MIN) {
                    __min_h_diff = 0;
                    __min_w_diff = 0;
                    __min_diff = __min_diff_tmp;
                    goto next_rate;
                }
//                if ((int) (__min_diff_tmp_f * 10) <= 0) {
//                    __min_h_diff = 0;
//                    __min_w_diff = 0;
//                    __min_diff_f = __min_diff_tmp_f;
//                    goto next_rate;
//                }
//
                /**
                 * bandingkan dengan nilai sebalumnya
                 */
                if (__min_diff_tmp < __min_diff) {
                    __min_h_diff = j;
                    __min_w_diff = k;
                    __min_diff = __min_diff_tmp;
//                    if (i == 0)
//                        printf("h:%d, w:%d, main diff: %d\n", __min_h_diff, __min_w_diff, __min_diff);
                }
//                if (__min_diff_tmp_f > __min_diff_f) {
//                    __min_h_diff = j;
//                    __min_w_diff = k;
//                    __min_diff_f = __min_diff_tmp_f;
////                    if (i == 0)
////                        printf("h:%d, w:%d, main diff: %d\n", __min_h_diff, __min_w_diff, __min_diff);
//                }

            }
        }

        next_rate:

        /**
         * jika __rate_h_offset sudah melebihi tinggi maksimum
         * generated BMP yang telah ditentukan, hentikan merge
         */
        if (__rate_h_offset + abs(__min_h_diff) > BMP_GENERATED_HEIGHT) break;

        /**
         * merge block sesuai shifting yang dihasilkan
         */
        block_merge(block_source, pixel_image_generate, SAMPLING_SIZE, (u_int8_t) width, __rate_h_offset,
                    (int16_t) (abs(__min_h_diff) - SAMPLING_SIZE), __min_w_diff, 255);

        /**
         * set offset untuk slice block selanjutnya
         */
        __rate_h_offset += abs(__min_h_diff);


//        printf("\nBlock-%d %d:%d = %d, offset: %d\n", (i + 1), __min_h_diff, __min_w_diff, __min_diff, __rate_h_offset);
//        printf("\nBlock-%d %d:%d = %f, offset: %d\n", i, __min_h_diff, __min_w_diff, __min_diff_f, __rate_h_offset);

        /**
         * free block
         * reset default max limit
         */
        free(block_source);
        free(block_target);
        __min_diff = 66585600;
//        __min_diff_f = 0;

//        if (i == 0) return -1;

    }
}

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
//                sum += 0;
                sum += abs((source[(i * width) + j]) << 2);
            else
                sum += abs((source[(i * width) + j] - target[((i + h_shift) * width) + (j + w_shift)]) << 2);

        }
    }

    return sum;
}

/**
 * Normalized Cross Correlation
 *
 * @param source
 * @param target
 * @param height
 * @param width
 * @param h_shift
 * @param w_shift
 * @return
 */
float NCC(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    u_int8_t i, j;
    u_int16_t length = height * width;
    float sum = 0, source_sum_squared = sum_squared(source, length, 0),
            target_sum_squared = sum_squared(target, length, h_shift * width);

    for (i = 0; i < height; i++) {

        for (j = 0; j < width; j++) {

            if (j + w_shift < 0 || j + w_shift > width - 1)
//                sum += 0;
                sum += source[(i * width) + j];
            else
                sum += source[(i * width) + j] * target[((i + h_shift) * width) + (j + w_shift)];

        }
    }

    return sum / sqrtf(source_sum_squared * target_sum_squared);
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

/**
 * Zero-mean Sum of Squared Differences
 *
 * @param source
 * @param target
 * @param height
 * @param width
 * @param h_shift
 * @param w_shift
 * @return
 */
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

/**
 * Zero-mean Normalised Cross Correlation
 *
 * @param source
 * @param target
 * @param height
 * @param width
 * @param h_shift
 * @param w_shift
 * @return
 */
float ZNCC(u_int8_t *source, u_int8_t *target, u_int16_t height, u_int16_t width, int8_t h_shift, int8_t w_shift) {

    u_int8_t i, j;
    u_int16_t length = height * width;
    float sum = 0, source_mean = mean(source, length, 0), target_mean = mean(target, length, h_shift * width),
            source_sum_mean_squared = sum_mean_squared(source, length, 0),
            target_sum_mean_squared = sum_mean_squared(target, length, h_shift * width);

    for (i = 0; i < height; i++) {

        for (j = 0; j < width; j++) {

            if (j + w_shift < 0 || j + w_shift > width - 1)
//                sum += 0;
                sum += source[(i * width) + j] - source_mean;
            else
                sum += (source[(i * width) + j] - source_mean) *
                       (target[((i + h_shift) * width) + (j + w_shift)] - target_mean);

        }
    }

    return sum / sqrtf(source_sum_mean_squared * target_sum_mean_squared);
}

/* Mean of Block array */
float mean(u_int8_t *block, u_int16_t length, u_int16_t shift) {
    u_int16_t i;
    float __sum = 0;
    for (i = 0; i < length; i++)
        __sum += *(block + i + shift);
    return __sum / length;
}

/* Sum of squared block array */
float sum_squared(u_int8_t *block, u_int16_t length, u_int16_t shift) {
    u_int16_t i;
    float __sum = 0;
    for (i = 0; i < length; i++)
        __sum += powf(*(block + i + shift), 2);
    return __sum;
}

float sum_mean_squared(u_int8_t *block, u_int16_t length, u_int16_t shift) {
    u_int16_t i;
    float __sum = 0, __mean = mean(block, length, shift);
    for (i = 0; i < length; i++)
        __sum += powf(*(block + i + shift) - __mean, 2);
    return __sum;
}