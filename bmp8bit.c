//
// Created by Ahmad Zafrullah on 9/19/16.
//
// Read and Generate BMP yang sama (verify)
//

#include <stdio.h>

#include "bmp.h"
#include "data.h"

#define     BMP_INFO_BIT                8
#define     BMP_COLOUR_INDEX_LENGTH     256


int main() {

    FILEHEADER fileheader;
    INFOHEADER infoheader;
    COLOURINDEX colourindex[BMP_COLOUR_INDEX_LENGTH];
    FILE *bmp_source_ptr, *bmp_target_ptr;

    /* open file */
    if ((bmp_source_ptr = fopen(BMP_SOURCE, "r")) == NULL) {
        printf("Failed to open %s\n", BMP_SOURCE);
        return -1;
    }

    /* header type */
    fread(&fileheader.type, sizeof(short), 1, bmp_source_ptr);
    if (fileheader.type != 0x4d42) {
        printf("File is not BMP (%x)\n", fileheader.type);
        return -1;
    }

    /* file header */
    fread(&fileheader.size, sizeof(int), 1, bmp_source_ptr);
    fread(&fileheader.reserved1, sizeof(short), 1, bmp_source_ptr);
    fread(&fileheader.reserved2, sizeof(short), 1, bmp_source_ptr);
    fread(&fileheader.offset, sizeof(int), 1, bmp_source_ptr);

    /* info header */
    fread(&infoheader, sizeof(INFOHEADER), 1, bmp_source_ptr);
    if (infoheader.bits != BMP_INFO_BIT) {
        printf("Only support BMP 8 bits\n");
    }

    /*  color index */
    fread(&colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_source_ptr);

    /* save image temporary */
    unsigned char grey[infoheader.imagesize];

    fseek(bmp_source_ptr, fileheader.offset, SEEK_SET);
    fread(&grey, sizeof(unsigned char), infoheader.imagesize, bmp_source_ptr);

    if ((bmp_target_ptr = fopen(BMP_TARGET, "wb")) == NULL) {
        printf("Failed to open %s", BMP_TARGET);
    }

    /* file header */
    fwrite(&fileheader.type, sizeof(short), 1, bmp_target_ptr);
    fwrite(&fileheader.size, sizeof(int), 1, bmp_target_ptr);
    fwrite(&fileheader.reserved1, sizeof(short), 1, bmp_target_ptr);
    fwrite(&fileheader.reserved2, sizeof(short), 1, bmp_target_ptr);
    fwrite(&fileheader.offset, sizeof(int), 1, bmp_target_ptr);

    /* info header */
    fwrite(&infoheader, sizeof(INFOHEADER), 1, bmp_target_ptr);

    /* color palette */
    fwrite(&colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_target_ptr);

    fseek(bmp_target_ptr, fileheader.offset, SEEK_SET);

    /* image data */
    fwrite(&grey, sizeof(unsigned char), infoheader.imagesize, bmp_target_ptr);

    return 0;
}