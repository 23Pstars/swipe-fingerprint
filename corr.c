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
 * Fungsi utama rekonstruksi gambar
 *
 * @param pixel_image
 * @param pixel_image_generate
 */
void reconstruct(unsigned char *pixel_image, unsigned char *pixel_image_generate) {

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
         * buffer untuk `block_a` (n) dan `block_b` (n-1)
         *
         */
        block_slice(pixel_image, block_a, i, BLOCK_SIZE);
        block_slice(pixel_image, block_b, (unsigned short) (i - BLOCK_HEIGHT), BLOCK_SIZE);

        /**
         * hitung dx dan dy
         */
        calculate_xy(block_a, block_b, &dy, &dx);

        /**
         * iterate offset digunakan pada proses merging
         */
        iterate_offset += dy;

        /**
         * @TODO
         * lakukan merging disini
         */
//        block_merge(pixel_image_generate, block_a, iterate_offset, dy, dx, 0);
//        printf("Offset-%d,\tdy: %d,\tdx: %d\n", i, dy, dx);

        /**
         * kosongkan buffer
         */
        memset(block_a, 0, BLOCK_SIZE);
        memset(block_b, 0, BLOCK_SIZE);
    }

    /**
     * bersihkan memory
     */
    free(block_a);
    free(block_b);

}

/**
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
    unsigned int diff = 66585600, diff_temp = 0;

    /**
     * variabel iterator
     */
    unsigned short i, j;

    /**
     * iterasi height, BLOCK_HEIGHT
     */
    for (i = 0; i < BLOCK_HEIGHT; i++) {

        /**
         * iterasi width, -WIDTH_RANGE_OFFSET : WIDTH_RANGE_OFFSET
         */
        for (j = -WIDTH_RANGE_OFFSET; j <= WIDTH_RANGE_OFFSET; j++) {

            /**
             * hitung diff antara 2 block
             * offset i dan j
             */
            diff_temp = SAD(block_a, block_b, BLOCK_HEIGHT, BMP_INPUT_WIDTH, (char) i, (char) j);


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
            }

            printf("\nBlock-%d %d:%d = %d\n", (i + 1), *dy, *dx, diff_temp);


        }

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
unsigned int
SAD(unsigned char *source, unsigned char *target, unsigned short height, unsigned short width, char h_shift,
    char w_shift) {

    short i, j;
    unsigned int sum = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {

            /**
             * jika ada over-index pada height
             */
            if (i + h_shift < 0 || i + h_shift > height - 1

                /**
                 * jika ada over-index pada width
                 */
                || j + w_shift < 0 || j + w_shift > width - 1)

                /**
                 * set rule yang akan diberikan
                 */
//                sum += 0;
                sum += target[(i * width) + j];

            else
                sum += abs(source[((i + h_shift) * width) + (j + w_shift)] - target[(i * width) + j]);

            /**
             * versi 2
             * shift vertical tetap mengambil dari memory sebelumnya,
             * jadi yang dihitung hanya shift horizontal saja
             */
//            if (j + w_shift < 0 || j + w_shift > width - 1)
////                sum += 0;
//                sum += source[(i * width) + j];
//            else
//                sum += abs(source[(i * width) + j] - target[((i + h_shift) * width) + (j + w_shift)]);

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
unsigned int
SSD(unsigned char *source, unsigned char *target, unsigned short height, unsigned short width, char h_shift,
    char w_shift) {

    short i, j;
    unsigned int sum = 0;

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
float NCC(unsigned char *source, unsigned char *target, unsigned short height, unsigned short width, char h_shift,
          char w_shift) {

    unsigned char i, j;
    unsigned short length = height * width;
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
float ZSAD(unsigned char *source, unsigned char *target, unsigned short height, unsigned short width, char h_shift,
           char w_shift) {

    unsigned char i, j;
    unsigned short length = height * width;
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
float ZSSD(unsigned char *source, unsigned char *target, unsigned short height, unsigned short width, char h_shift,
           char w_shift) {

    unsigned char i, j;
    unsigned short length = height * width;
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
float ZNCC(unsigned char *source, unsigned char *target, unsigned short height, unsigned short width, char h_shift,
           char w_shift) {

    unsigned char i, j;
    unsigned short length = height * width;
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
float mean(unsigned char *block, unsigned short length, unsigned short shift) {
    unsigned short i;
    float __sum = 0;
    for (i = 0; i < length; i++)
        __sum += *(block + i + shift);
    return __sum / length;
}

/* Sum of squared block array */
float sum_squared(unsigned char *block, unsigned short length, unsigned short shift) {
    unsigned short i;
    float __sum = 0;
    for (i = 0; i < length; i++)
        __sum += powf(*(block + i + shift), 2);
    return __sum;
}

float sum_mean_squared(unsigned char *block, unsigned short length, unsigned short shift) {
    unsigned short i;
    float __sum = 0, __mean = mean(block, length, shift);
    for (i = 0; i < length; i++)
        __sum += powf(*(block + i + shift) - __mean, 2);
    return __sum;
}