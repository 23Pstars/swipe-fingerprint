/**
 * BMP meta info
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

#define BMP_SOURCE "/Users/zaf/Google Drive/Kuliah/Universitas Gadjah Mada (S2)/thesis/v2/apps/BMP/GENERATED_BMP.bmp"

int main() {

    FILEHEADER fh;
    INFOHEADER ih;
    COLOURINDEX ci[128];

    read_bmp_header(&fh, &ih, ci);

    printf("\nsize\nfh: %zu\n", sizeof(fh));
    printf("ih: %zu\n", sizeof(ih));
    printf("ci: %zu\n\n\n", sizeof(ci));

    printf("fh offset: %d\n", fh.offset);
    printf("ih width: %d\n", ih.width);
    printf("ih height: %d\n", ih.height);
    printf("if imagesize: %d\n\n", ih.imagesize);

    printf("fh size: %d\n", fh.size);
    printf("calculated size: %d\n", (int) (14 + 40 + (128 * sizeof(COLOURINDEX)) + (ih.width * ih.height)));

}