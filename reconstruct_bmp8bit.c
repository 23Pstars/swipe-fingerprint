/**
 * Ahmad Zafrullah
 * http://zaf.web.id
 *
 * Universitas Gadjah Mada
 *
 * Rekonstruksi overlapping BMP image
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bmp.h"
#include "correlation_algorithm.h"
#include "data.h"

#define     BMP_INFO_BIT                8
#define     BMP_COLOUR_INDEX_LENGTH     256

/**
 * tinggi (h_size) tiap block
 */
#define     SAMPLING_SIZE               8

/**
 * offset tiap pencocokan apakah geser kiri,
 * kanan, atas, atau bawah
 */
#define     HEIGHT_RANGE_OFFSET         8
#define     WIDTH_RANGE_OFFSET          8

/**
 * jika treshold sudah terpenuhi program tidak perlu
 * lagi untuk melakukan looping
 */
#define     DIFF_THRESHOLD_MIN          0

/**
 * perlu dibatasi differences maksimal antar block
 * karena bisa jadi block tersebut memang berurutan
 * (tidak overlapping)
 */
#define     DIFF_THRESHOLD_MAX          0

int main() {

    clock_t __time_start = clock();

    FILEHEADER fileheader;
    INFOHEADER infoheader;
    COLOURINDEX colourindex[BMP_COLOUR_INDEX_LENGTH];
    FILE *bmp_source_ptr, *bmp_target_ptr;

    /**
     * mulai membuka file
     */
    if ((bmp_source_ptr = fopen(BMP_SOURCE, "r")) == NULL) {
        printf("Failed to open %s\n", BMP_SOURCE);
        return -1;
    }

    /**
     * deteksi jenis BMP
     */
    fread(&fileheader.type, 2, 1, bmp_source_ptr);
    if (fileheader.type != 0x4d42) {
        printf("File is not BMP (%x)\n", fileheader.type);
        return -1;
    }

    /**
     * baca file header
     */
    fread(&fileheader.size, 4, 1, bmp_source_ptr);
    fread(&fileheader.reserved1, 2, 1, bmp_source_ptr);
    fread(&fileheader.reserved2, 2, 1, bmp_source_ptr);
    fread(&fileheader.offset, 4, 1, bmp_source_ptr);

    /**
     * baca info header
     */
    fread(&infoheader, sizeof(INFOHEADER), 1, bmp_source_ptr);
    if (infoheader.bits != BMP_INFO_BIT) {
        printf("Only support BMP 8 bits\n");
        return -1;
    }

    /**
     * color index (opsional)
     */
    fread(&colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_source_ptr);

    /**
     * simpan gambar sementara
     */
    u_int8_t pixel_image[infoheader.imagesize],
            pixel_image_reversed[infoheader.imagesize];

    u_int8_t pixel_image_generate[infoheader.imagesize];

    /**
     * @todo
     * isi dengan warna putih sementara, agar terlihat
     * block yang sudah terisi dan yang belum terisi
     */
    memset(pixel_image_generate, 255, infoheader.imagesize);

    fseek(bmp_source_ptr, fileheader.offset, SEEK_SET);
    fread(&pixel_image, 1, infoheader.imagesize, bmp_source_ptr);

    /**
     * perlu dilakukan reverse order
     * karena kebetulan gambar output terbalik tiap 8 pixel-row
     */
    block_reverse(pixel_image, pixel_image_reversed, infoheader.height, infoheader.width, SAMPLING_SIZE);

    /**
     * ======================================
     *   Mulai pendeteksian block of pixel
     * ======================================
     */


    u_int32_t __min_diff = 66585600,    /// perbedaan terkecil dari tiap block (max 255^2 x 128 x 8 = 66585600)
            __min_diff_tmp = 0;         /// perbedaan sementara sebagai pembanding dengan __min_diff

//    float __min_diff_f = 0, __min_diff_tmp_f = 0;

    int8_t j, k;                        /// iterasi shift horizontal dan vertical

    int16_t i,                          /// untuk keperluan iterasi saja (range -32768 : 32767)
            __min_h_diff,               /// jarak vertical dengan perbedaan terkecil
            __min_w_diff,               /// jarak horizontal dengan perbedaan terkecil

            __block_size = (u_int16_t) (SAMPLING_SIZE * infoheader.width),    /// jumlah byte dalam 1 block
            __rate = (u_int16_t) (infoheader.imagesize / __block_size);       /// jumlah keseluruhan block

    int16_t __rate_h_offset = SAMPLING_SIZE;         /// offset h setelah block di-merge

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
    memcpy(pixel_image_generate, pixel_image_reversed, __block_size);

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
        u_int8_t *block_source = block_slice(pixel_image_reversed, infoheader.height, infoheader.width,
                                             SAMPLING_SIZE,
                                             (u_int16_t) infoheader.width,
                                             (u_int16_t) ((i + 1) * SAMPLING_SIZE), 0);

        /**
         * berisi template untuk pembanding, 2 x block source
         * ukuran 2 x SAMPLING x width
         */
        u_int8_t *block_target = block_slice(pixel_image_reversed, infoheader.height, infoheader.width,
                                             2 * SAMPLING_SIZE,
                                             (u_int16_t) infoheader.width,
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
                __min_diff_tmp = (u_int32_t) SAD(block_source, block_target, SAMPLING_SIZE,
                                                 (u_int16_t) infoheader.width,
                                                 (int8_t) (j + SAMPLING_SIZE), (int8_t) k);

                /**
                 * - NCC (best maximum)
                 * - ZNNC (best maximum)
                 */
//                __min_diff_tmp_f = NCC(block_source, block_target, SAMPLING_SIZE,
//                                       (u_int16_t) infoheader.width,
//                                       (int8_t) (j + SAMPLING_SIZE), (int8_t) k);

                /**
                 * @todo
                 * bagaimana jika memang tidak ada korelasi dari kedua block tersebut?
                 * (DIFF_THRESHOLD_MAX)
                 *
                 * jika memenuhi treshold lanjutkan
                 * ke __rate selanjutnya
                 */
                if (__min_diff_tmp <= DIFF_THRESHOLD_MIN) {
                    __min_diff = __min_diff_tmp;
                    goto next_rate;
                }

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
         * merge block sesuai shifting yang dihasilkan
         */
        block_merge(block_source, pixel_image_generate, SAMPLING_SIZE, (u_int8_t) infoheader.width,
                    __rate_h_offset, __min_h_diff, __min_w_diff, 127);

        /**
         * set offset untuk slice block selanjutnya
         */
        __rate_h_offset += __min_h_diff + SAMPLING_SIZE;


        printf("\nBlock-%d %d:%d = %d, offset: %d\n", i, __min_h_diff, __min_w_diff, __min_diff, __rate_h_offset);
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

    /* WRITE IT AGAIN */

    if ((bmp_target_ptr = fopen(BMP_TARGET, "wb")) == NULL) {
        printf("Failed to open %s", BMP_TARGET);
    }

    /* file header */
    fwrite(&fileheader.type, 2, 1, bmp_target_ptr);
    fwrite(&fileheader.size, 4, 1, bmp_target_ptr);
    fwrite(&fileheader.reserved1, 2, 1, bmp_target_ptr);
    fwrite(&fileheader.reserved2, 2, 1, bmp_target_ptr);
    fwrite(&fileheader.offset, 4, 1, bmp_target_ptr);

    /* info header */
    fwrite(&infoheader, sizeof(INFOHEADER), 1, bmp_target_ptr);

    /* color palette */
    fwrite(&colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_target_ptr);

    fseek(bmp_target_ptr, fileheader.offset, SEEK_SET);

    /* image data */
    fwrite(&pixel_image_generate, 1, infoheader.imagesize, bmp_target_ptr);

    clock_t __time_end = clock();
    double __time_execution = (double) (__time_end - __time_start) / CLOCKS_PER_SEC;

    printf("\nExecution time: %.2f seconds", __time_execution);

    return 0;
}