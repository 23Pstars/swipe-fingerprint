/**
 * BMP meta info
 */

#include <stdio.h>

#include "bmp.h"

#define     TEST_BMP   "/Users/zaf/thesis-apps/swipe-fingerprint/assets/8RUL2001.bmp"

int main() {

    FILEHEADER fh;
    INFOHEADER ih;
    COLOURINDEX ci[128];

    FILE *bmp_source_ptr = open_bmp_file(TEST_BMP, "r");

    fread(&fh.type, 2, 1, bmp_source_ptr);
    fread(&fh.size, 4, 1, bmp_source_ptr);
    fread(&fh.reserved1, 2, 1, bmp_source_ptr);
    fread(&fh.reserved2, 2, 1, bmp_source_ptr);
    fread(&fh.offset, 4, 1, bmp_source_ptr);
    fread(&ih, sizeof(INFOHEADER), 1, bmp_source_ptr);
    fread(&ci, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_source_ptr);

    printf("\npath\n%s\n", TEST_BMP);

    printf("\nsize\nfh: %zu\n", sizeof(fh));
    printf("ih: %zu\n", sizeof(ih));
    printf("ci: %zu\n\n\n", sizeof(ci));

    printf("fh size: %d\n", fh.size);
    printf("fh offset: %d\n", fh.offset);

    printf("ih size: %d\n", ih.size);
    printf("ih width: %d\n", ih.width);
    printf("ih height: %d\n", ih.height);
    printf("if imagesize: %d\n\n", ih.imagesize);

    printf("fh size: %d\n", fh.size);
    printf("calculated size: %d\n", (int) (14 + 40 + (128 * sizeof(COLOURINDEX)) + (ih.width * ih.height)));

}