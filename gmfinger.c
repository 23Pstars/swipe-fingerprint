#include <stdio.h>
#include <stdlib.h>

#include "../Share-Libs/BLFingerAPI.h"

#define     RECONSTRUCTED_WIDTH             128
#define     RECONSTRUCTED_HEIGHT            400
#define     RECONSTRUCTED_SIZE              RECONSTRUCTED_WIDTH * RECONSTRUCTED_HEIGHT
#define     RECONSTRUCTED_FILE_1            "/home/zaf/thesis-apps/swipe-fingerprint/assets/HC/HCL0000.bmp"
#define     RECONSTRUCTED_FILE_2            "/home/zaf/thesis-apps/swipe-fingerprint/assets/HC/HCL5000.bmp"

#define     FEATURE_DATA_SIZE               1024*256
#define     FEATURE_DATA_FILE_1             "/home/zaf/thesis-apps/swipe-fingerprint/assets/FD1.txt"
#define     FEATURE_DATA_FILE_2             "/home/zaf/thesis-apps/swipe-fingerprint/assets/FD2.txt"

int main() {

    printf("Initialize: %d\n", GMFAPI_Initialize());

    char *GMAPI_Version = malloc(200 * sizeof(char));

    GMAPI_GetVersion(GMAPI_Version);

    printf("Version: %s\n", GMAPI_Version);

    unsigned char *pixel_image_1 = malloc(sizeof(char) * RECONSTRUCTED_SIZE);
    unsigned char *pixel_image_2 = malloc(sizeof(char) * RECONSTRUCTED_SIZE);

    FILE *bmp_ptr_1, *bmp_ptr_2;
    if ((bmp_ptr_1 = fopen(RECONSTRUCTED_FILE_1, "rb")) == NULL ||
        (bmp_ptr_2 = fopen(RECONSTRUCTED_FILE_2, "rb")) == NULL) {
        printf("Failed to open %s and %s\n", RECONSTRUCTED_FILE_1, RECONSTRUCTED_FILE_2);
        exit(-1);
    }

    fseek(bmp_ptr_1, 1078, SEEK_SET);
    fread(pixel_image_1, 1, RECONSTRUCTED_SIZE, bmp_ptr_1);

    fseek(bmp_ptr_2, 1078, SEEK_SET);
    fread(pixel_image_2, 1, RECONSTRUCTED_SIZE, bmp_ptr_2);

    unsigned char *pFD_1 = malloc(FEATURE_DATA_SIZE * sizeof(char));
    printf("Extract FD1: %d\n", GMFAPI_Extraction(pixel_image_1, pFD_1));
    unsigned char *pFD_2 = malloc(FEATURE_DATA_SIZE * sizeof(char));
    printf("Extract FD2: %d\n", GMFAPI_Extraction(pixel_image_2, pFD_2));

    printf("FD1 vs FD2: %d\n", GMFAPI_Matching(pFD_1, pFD_2));

    FILE *txt_ptr_1, *txt_ptr_2;
    if ((txt_ptr_1 = fopen(FEATURE_DATA_FILE_1, "wb")) == NULL ||
        (txt_ptr_2 = fopen(FEATURE_DATA_FILE_2, "wb")) == NULL) {
        printf("Failed to open %s\n", FEATURE_DATA_FILE_1);
        exit(-1);
    }

    unsigned short h;
    for (h = 0; h < 1024; h++) {
//        fprintf(txt_ptr_1, "%d\t%s", *(pixel_image_1 + h), ((h + 1) % RECONSTRUCTED_WIDTH == 0 ? "\n" : ""));
//        fprintf(txt_ptr_2, "%d\t%s", *(pixel_image_2 + h), ((h + 1) % RECONSTRUCTED_WIDTH == 0 ? "\n" : ""));
        fprintf(txt_ptr_1, "%d ", *(pFD_1 + h));
        fprintf(txt_ptr_2, "%d ", *(pFD_2 + h));
    }

    return 0;

}
