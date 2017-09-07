#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#include "../Share-Libs/BLFingerAPI.h"

#define     RECONSTRUCTED_WIDTH             126
#define     RECONSTRUCTED_HEIGHT            400
#define     RECONSTRUCTED_SIZE              (RECONSTRUCTED_WIDTH * RECONSTRUCTED_HEIGHT)
#define     RECONSTRUCTED_DIR               "/home/zaf/thesis-apps/swipe-fingerprint/assets/results/"

#define     FEATURE_DATA_SIZE               (4096*4096)

int main(int argc, char *argv[]) {

    clock_t time_start = clock();

    GMFAPI_Initialize();

    unsigned char *pixel_image_1 = malloc(sizeof(char) * RECONSTRUCTED_SIZE);
    unsigned char *pixel_image_2 = malloc(sizeof(char) * RECONSTRUCTED_SIZE);

    unsigned short h;
    unsigned char pad = RECONSTRUCTED_WIDTH & 3;

    FILE *bmp_ptr_1, *bmp_ptr_2;
    char file1[256], file2[256];

    sprintf(file1, "%s%s/%s/HA/HAL0000.bmp", RECONSTRUCTED_DIR, argv[1], argv[2]);
    sprintf(file2, "%s%s/%s/HA/HAL1000.bmp", RECONSTRUCTED_DIR, argv[1], argv[2]);

    if ((bmp_ptr_1 = fopen(file1, "rb")) == NULL ||
        (bmp_ptr_2 = fopen(file2, "rb")) == NULL) {
        printf("Failed to open %s and %s\n", file1, file2);
        exit(-1);
    }

    fseek(bmp_ptr_1, 1078, SEEK_SET);
    fseek(bmp_ptr_2, 1078, SEEK_SET);

    for (h = 0; h < RECONSTRUCTED_HEIGHT; h++) {
        fread(pixel_image_1 + (RECONSTRUCTED_WIDTH * h), RECONSTRUCTED_WIDTH, 1, bmp_ptr_1);
        fseek(bmp_ptr_1, pad, SEEK_CUR);

        fread(pixel_image_2 + (RECONSTRUCTED_WIDTH * h), RECONSTRUCTED_WIDTH, 1, bmp_ptr_2);
        fseek(bmp_ptr_2, pad, SEEK_CUR);
    }

    unsigned char *pFD_1 = calloc(FEATURE_DATA_SIZE, sizeof(char));
    GMFAPI_Extraction(pixel_image_1, pFD_1);
    unsigned char *pFD_2 = calloc(FEATURE_DATA_SIZE, sizeof(char));
    GMFAPI_Extraction(pixel_image_2, pFD_2);


    fclose(bmp_ptr_1);
    fclose(bmp_ptr_2);

//    printf("%d\n", GMFAPI_Matching(pFD_1, pFD_2));
    printf("%f", (double) (clock() - time_start) / CLOCKS_PER_SEC);

    return 0;

}
