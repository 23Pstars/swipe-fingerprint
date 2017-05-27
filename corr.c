/**
 * source untuk daftar algoritma correlation
 * berbasis array (pointer) 1 dimensi (vektor)
 */

#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "corr.h"
#include "bmp.h"

/**
 * Fungsi utama rekonstruksi gambar
 *
 * @param pixel_image
 * @param pixel_image_generate
 */
void reconstruct(unsigned char *pixel_image, unsigned char *pixel_image_generate) {

    /**
     * menyimpan nilai dx dan dy sejumlah JUMPING_RANGE
     * nilai sebelumnya
     */
    short j = 0, *prev_dy = malloc(JUMPING_RANGE * sizeof(short));

    /**
     * iterasi tiap block
     */
    unsigned short i;

    /**
     * buffer untuk dx dan dy
     * range -128 : 127
     */
    short dy, dx, iterate_offset = 0;

    /**
     * buffer 2 block yang akan dibandingkan
     * block_a akan dijadikan pembanding
     * block_b akan menjadi template (block sebelumnya),
     * yakni seberapa besar pergeseran dari dari block n-1 ke block ns
     */
    unsigned char *block_a = malloc(sizeof(unsigned char) * BLOCK_SIZE),
            *block_b = malloc(sizeof(unsigned char) * BLOCK_SIZE);

    /**
     * copy block #0 pada `pixel_image` ke `pixel_image_generate`
     */
    block_slice(pixel_image, block_a, 0, BLOCK_SIZE);
    memcpy(pixel_image_generate, block_a, BLOCK_SIZE);
    memset(block_a, 0, BLOCK_SIZE);

    /**
     * iterasi untuk setiap block, mulai dari #1
     * akhir iterasi dikurangi 1 block, karena block_b
     * memiliki porsi 2 block sebagai template
     */
    for (i = BLOCK_HEIGHT; i < BMP_INPUT_HEIGHT - BLOCK_HEIGHT; i += BLOCK_HEIGHT) {

        /**
         * buffer untuk `block_a` (i) dan `block_b` (i-1)
         */
        block_slice(pixel_image, block_a, i, BLOCK_SIZE);
        block_slice(pixel_image, block_b, (unsigned short) (i - BLOCK_HEIGHT), BLOCK_SIZE);

        /**
         * jika block a dan b sama, tidak perlu dicek
         * overlappong nya
         */
        if (memcmp(block_a, block_b, BLOCK_SIZE) == 0)
            continue;

        /**
         * hitung dx dan dy
         */
        if (!USE_JUMPING) {
            calculate_xy(block_a, block_b, &dy, &dx);

        } else if (j == JUMPING_RANGE || j < 0) {
            dy = average_dy(prev_dy);
            dx = WIDTH_RANGE_OFFSET;
            if (j == JUMPING_RANGE)
                j = -JUMPING_COUNT;
        } else {
            calculate_xy(block_a, block_b, &dy, &dx);
            *(prev_dy + j) = dy;
        }

        if (dy == 0 && abs(dx) == WIDTH_RANGE_OFFSET)
            continue;

        /**
         * iterate offset digunakan pada proses merging
         */
        iterate_offset += dy;

        /**
         * merge block sesuai dx dan dy
         */
        block_merge(pixel_image_generate, block_a, BLOCK_HEIGHT, BMP_INPUT_WIDTH, iterate_offset, 0, dx);
//        printf("Offset-%d, Iterate-%d\tdy: %d,\tdx: %d\tprev_xy(%d): %d\n", i, iterate_offset, dy, dx, j,
//               *(prev_dy + j));

        /**
         * kosongkan buffer
         */
        memset(block_a, 0, BLOCK_SIZE);
        memset(block_b, 0, BLOCK_SIZE);

        j++;

    }

    /**
     * bersihkan memory
     *
     * untuk melakukan rekonstruksi banyak gambar sekaligus,
     * fungsi `free()` sering membuat error
     */
    free(block_a);
    free(block_b);

}

/**
 * melakukan looping untuk mencari dx dan dy minimum
 *
 * @param block_a
 * @param block_b
 * @param dx
 * @param dy
 */
void calculate_xy(unsigned char *block_a, unsigned char *block_b, short *dy, short *dx) {

    /**
     * perbedaan terkecil dari tiap block (max 255^2 x 128 x 8 = 66585600)
     * 255^2 berpatok pada SSD dengan kemungkinan terbesar = (250-0)^2
     */
    unsigned int diff = 0xffffffff, diff_temp;

    /**
     * variabel iterator
     */
    short i, j;

    /**
     * iterasi height, BLOCK_HEIGHT
     * sesuai gambaran yang ditulis dibuku
     *
     * update: range mulai dari H- ke H+
     * @TODO: optimasi kembali
     */
    for (i = 0; i <= HEIGHT_RANGE_OFFSET; i++) {

        /**
         * iterasi width, -WIDTH_RANGE_OFFSET : WIDTH_RANGE_OFFSET
         */
        for (j = -WIDTH_RANGE_OFFSET; j <= WIDTH_RANGE_OFFSET; j++) {

            /**
             * hitung diff antara 2 block
             * offset i dan j
             */
            diff_temp = diff_xy(block_a, block_b, BLOCK_HEIGHT, BMP_INPUT_WIDTH, (char) i, (char) j);

            /**
             * agar tidak boros iterasi, jika sudah mencapai nilai tertentu
             * lanjut ke tahap berikutnya
             */
            if (diff_temp <= DIFF_THRESHOLD_MIN) {
                *dy = i;
                *dx = j;
                return;
            }

            /**
             * update jika diff yang didapat lebih kecil
             * dari diff sebelumnya
             */
            if (diff_temp < diff) {
                *dy = i;
                *dx = j;
                diff = diff_temp;
            }

        }

    }
}

/**
 * mencari minimum difference dari 2 block
 * sesuai height dan width offset
 *
 * @param block_a
 * @param block_b
 * @param block_height
 * @param block_width
 * @param height_shift
 * @param width_shift
 * @return
 */
unsigned int diff_xy(unsigned char *block_a, unsigned char *block_b,
                     unsigned short block_height, unsigned short block_width,
                     char height_shift, char width_shift) {

    short i, j;
    unsigned int sum = 0;

    /**
     * iterasi height
     * total height menjadi lebih besar karena ada tambahan
     * space dari shift
     */
    for (i = 0; i < block_height + abs(height_shift); i++) {

        /**
         * iterasi width
         * total width juga menjadi lebar
         */
        for (j = 0; j < block_width + abs(width_shift); j++) {

            if (height_shift > 0) {

                if (width_shift > 0) {

                    /**
                     * height_shift +
                     * width_shift +
                     */

                    if ((i - height_shift < 0 && j >= block_width) || (i >= block_height && j - width_shift < 0)) {

                    } else if (i - height_shift < 0 || j - width_shift < 0) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(0, block_b[(i * block_width) + j]);
                    } else if (i >= block_height || j >= block_width) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(block_a[((i - height_shift) * block_width) + j - width_shift], 0);
                    } else {
                        sum += apply_algo(block_a[((i - height_shift) * block_width) + j - width_shift],
                                          block_b[(i * block_width) + j]);
                    }

                } else {

                    /**
                     * height_shift +
                     * width_shift -
                     */

                    if ((i - height_shift < 0 && j + width_shift < 0) || (j >= block_width && i >= block_height)) {

                    } else if (i - height_shift < 0 || j >= block_width) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(0, block_b[(i * block_width) + j + width_shift]);
                    } else if (j + width_shift < 0 || i >= block_height) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(block_a[((i - height_shift) * block_width) + j], 0);
                    } else {
                        sum += apply_algo(block_a[((i - height_shift) * block_width) + j],
                                          block_b[(i * block_width) + j + width_shift]);
                    }

                }

            } else {

                if (width_shift > 0) {

                    /**
                     * height_shift -
                     * width_shift +
                     */

                    if ((i + height_shift < 0 && j - width_shift < 0) || (i >= block_height && j >= block_width)) {

                    } else if (i + height_shift < 0 || j >= block_width) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(block_a[(i * block_width) + j - width_shift], 0);
                    } else if (j - width_shift < 0 || i >= block_height) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(0, block_b[((i + height_shift) * block_width) + j]);
                    } else {
                        sum += apply_algo(block_a[(i * block_width) + j - width_shift],
                                          block_b[((i + height_shift) * block_width) + j]);
                    }

                } else {

                    /**
                     * height_shift -
                     * width_shift -
                     */

                    if ((i + height_shift < 0 && j >= block_width) || (i >= block_height && j + width_shift < 0)) {

                    } else if (i + height_shift < 0 || j + width_shift < 0) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(block_a[(i * block_width) + j], 0);
                    } else if (i >= block_height || j >= block_width) {
//                        if (SUM_OVERLAP_INDEX)
//                            sum += apply_algo(0, block_b[((i + height_shift) * block_width) + j + width_shift]);
                    } else {
                        sum += apply_algo(block_a[(i * block_width) + j],
                                          block_b[((i + height_shift) * block_width) + j + width_shift]);
                    }

                }

            }

        }

    }

    /**
     * Lakukan normalisasi
     * - tipe fungsi `int`
     * - dibagi dengan luasan overlapping area
     */
    return (sum * 4096) / ((BMP_INPUT_WIDTH - abs(width_shift)) * (BLOCK_HEIGHT - abs(height_shift)));

}

/**
 * kostumisasi untuk implementasi algoritma
 * yang akan digunakan
 *
 * @param a
 * @param b
 * @return
 */
unsigned int apply_algo(unsigned char a, unsigned char b) {
    switch (USE_ALGO) {
        case 1:
            return SAD_algo(a, b);
        case 2:
            return SSD_algo(a, b);
        default:
            return 0;
    }
}



/** ========================
 *    DIFFERENCE ALGORITHM
 * ======================== */

/**
 * Sum of Absolute Differences
 *
 * @param a
 * @param b
 * @return
 */
unsigned int SAD_algo(unsigned char a, unsigned char b) {
    return (unsigned int) abs(a - b);
}

/**
 * Sum of Squared Differences
 *
 * @param a
 * @param b
 * @return
 */
unsigned int SSD_algo(unsigned char a, unsigned char b) {
    return (unsigned int) abs((a - b) << 2);
}

/**
 * @TODO
 * Normalized Cross Correlation
 *
 * @param a
 * @param b
 * @param u
 * @return
 */
float NCC_algo(unsigned char a, unsigned char b, float u) {
    return 0;
}

short average_dy(short *prev_dy) {
    short sum = 0;
    unsigned char i;
    for (i = 0; i < JUMPING_RANGE; i++)
        sum += *(prev_dy + i);
    return (short) ((double) sum / JUMPING_RANGE);
}