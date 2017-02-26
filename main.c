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

#include "config.h"
#include "bmp.h"
#include "corr.h"

int main() {

    BMPHEADER bmpheader;
    unsigned char
            *pixel_image = calloc(BMP_INPUT_SIZE, sizeof(unsigned char)),
            *pixel_image_reversed = calloc(BMP_INPUT_SIZE, sizeof(unsigned char)),
            *pixel_image_generate = calloc(BMP_OUTPUT_SIZE, sizeof(unsigned char));

    load_bmp(&bmpheader, pixel_image);

    block_reverse(pixel_image, pixel_image_reversed);

    reconstruct(pixel_image_reversed, pixel_image_reversed);

    save_bmp(&bmpheader, pixel_image_reversed);

    free(pixel_image);
    free(pixel_image_reversed);
    free(pixel_image_generate);

    return 0;
}