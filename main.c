/**
 * Ahmad Zafrullah
 * https://zaf.web.id
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
#include "corr.h"
#include "config.h"

int main() {

    clock_t __time_start = clock();

    /**
     * struktur BMP
     *
     * @todo
     * remember to free this vars
     */
    FILEHEADER fileheader;
    INFOHEADER infoheader;
    COLOURINDEX colourindex[BMP_COLOUR_INDEX_LENGTH];

    /**
     * baca pixel gambar
     */
    read_bmp_header(&fileheader, &infoheader, colourindex);

    /**
     * generated image size
     */
    u_int32_t __image_size_generated = (u_int32_t) (infoheader.width * BMP_GENERATED_HEIGHT);

    /**
     * buffer
     */
    u_int8_t *pixel_image = malloc(sizeof(u_int8_t) * infoheader.imagesize),
//            *pixel_image_reversed = malloc(sizeof(u_int8_t) * infoheader.imagesize),
            *pixel_image_generate = malloc(sizeof(u_int8_t) * __image_size_generated);

    /**
     * read pixel image data
     */
    read_bmp_pixel_image(pixel_image, infoheader.imagesize, fileheader.offset);

    /**
     * isi block sisanya dengan hitam
     */
    memset(pixel_image_generate, 0, __image_size_generated);

    /**
     * perlu dilakukan reverse order
     * karena kebetulan gambar output terbalik tiap 8 pixel-row
     */
//    block_reverse(pixel_image, pixel_image_reversed, infoheader.height, infoheader.width, SAMPLING_SIZE);
//    write_bmp(&fileheader, &infoheader, colourindex, pixel_image_reversed);

    /**
     * mulai rekonstruksi
     */
    reconstruct(pixel_image, pixel_image_generate, infoheader.height, infoheader.width);

    /**
     * Customize with expected height of image
     */
    infoheader.height = BMP_GENERATED_HEIGHT;
    infoheader.imagesize = __image_size_generated;

    /**
     * fileheader: 14 bytes
     * infoheader: 40 bytes
     * colourindex: 128 * 8 bytes
     */
    fileheader.size = 54 + sizeof(colourindex) + __image_size_generated;

    /**
     * tulis kembali file BMP
     */
    write_bmp(&fileheader, &infoheader, colourindex, pixel_image_generate);

    clock_t __time_end = clock();
    double __time_execution = (double) (__time_end - __time_start) / CLOCKS_PER_SEC;

    printf("\nExecution time: %.10f seconds", __time_execution);

    return 0;
}