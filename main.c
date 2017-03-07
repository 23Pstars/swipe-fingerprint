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
#include <memory.h>
#include <time.h>

#include "config.h"
#include "bmp.h"
#include "corr.h"

int main() {

    clock_t time_start = clock();

    BMPHEADER bmpheader;
    unsigned char
            *pixel_image = malloc(BMP_INPUT_SIZE * sizeof(unsigned char)),
            *pixel_image_reversed = malloc(BMP_INPUT_SIZE * sizeof(unsigned char)),
            *pixel_image_generate = malloc(BMP_OUTPUT_SIZE * sizeof(unsigned char));

    memset(pixel_image_generate, DEFAULT_OVERLAP_VALUE, BMP_OUTPUT_SIZE);

    load_bmp(&bmpheader, pixel_image);

    block_reverse(pixel_image, pixel_image_reversed);

    reconstruct(pixel_image_reversed, pixel_image_generate);

    save_bmp(&bmpheader, pixel_image_generate);

    free(pixel_image);
    free(pixel_image_reversed);
    free(pixel_image_generate);

    printf("\nExecution time: %.10f seconds", (double) (clock() - time_start) / CLOCKS_PER_SEC);

    return 0;
}