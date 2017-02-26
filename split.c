/**
 * split to each block
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "config.h"
#include "bmp.h"

#define OUTPUT_PATH "/Users/zaf/Google Drive/Kuliah/Universitas Gadjah Mada (S2)/thesis/v2/apps/BMP/assets/block/block-"

int main() {

    char file_out[250];
    FILE *block_out;
    BMPHEADER bmpheader;
    unsigned char *block_a = malloc(sizeof(unsigned char) * BLOCK_SIZE),
            *pixel_image = calloc(BMP_INPUT_SIZE, sizeof(unsigned char)),
            *pixel_image_reversed = calloc(BMP_INPUT_SIZE, sizeof(unsigned char));

    load_bmp(&bmpheader, pixel_image);

    block_reverse(pixel_image, pixel_image_reversed);

    for (unsigned short i = 0; i < BMP_INPUT_HEIGHT; i += BLOCK_HEIGHT) {

        sprintf(file_out, "%s%d.bmp", OUTPUT_PATH, i);

        block_out = open_bmp_file(file_out, "wb");

        block_slice(pixel_image_reversed, block_a, i, BLOCK_SIZE);

        bmpheader.infoheader.height = BLOCK_HEIGHT;
        bmpheader.infoheader.imagesize = BLOCK_SIZE;
        bmpheader.fileheader.size = (unsigned int) (1078 + BLOCK_SIZE);

        fwrite(&bmpheader.fileheader.type, 2, 1, block_out);
        fwrite(&bmpheader.fileheader.size, 4, 1, block_out);
        fwrite(&bmpheader.fileheader.reserved1, 2, 1, block_out);
        fwrite(&bmpheader.fileheader.reserved2, 2, 1, block_out);
        fwrite(&bmpheader.fileheader.offset, 4, 1, block_out);

        fwrite(&bmpheader.infoheader, sizeof(INFOHEADER), 1, block_out);
        fwrite(&bmpheader.colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, block_out);

        fseek(block_out, bmpheader.fileheader.offset, SEEK_SET);
        fwrite(block_a, 1, bmpheader.infoheader.imagesize, block_out);

        fclose(block_out);
        memset(block_a, 0, BLOCK_SIZE);

    }

    return 0;
}