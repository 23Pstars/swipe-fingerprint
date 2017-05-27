#include <stdio.h>
#include <stdlib.h>

#include "../Share-Libs/BLFingerAPI.h"

#define     JUMP                            1

#define     RECONSTRUCTED_WIDTH             126
#define     RECONSTRUCTED_HEIGHT            400
#define     RECONSTRUCTED_SIZE              (RECONSTRUCTED_WIDTH * RECONSTRUCTED_HEIGHT)
#define     RECONSTRUCTED_DIR               "/home/zaf/thesis-apps/swipe-fingerprint/assets/results/2/"

#define     FEATURE_DATA_SIZE               (4096*4096)

int main() {

    printf("Initialize: %d\n", GMFAPI_Initialize());

    char *GMAPI_Version = malloc(200 * sizeof(char));

    GMAPI_GetVersion(GMAPI_Version);

    printf("Version: %s\n", GMAPI_Version);

    unsigned char *pixel_image_1 = malloc(sizeof(char) * RECONSTRUCTED_SIZE);
    unsigned char *pixel_image_2 = malloc(sizeof(char) * RECONSTRUCTED_SIZE);

    unsigned short h;
    unsigned char pad = RECONSTRUCTED_WIDTH & 3;

    FILE *bmp_ptr_1, *bmp_ptr_2;
    char file1[256], file2[256];
    unsigned char i, j, k, l, f = 3, fn = 6;
    unsigned int genuine = 0, impostor = 0;

    // finger
    for (i = 0; i < f; i++) {

        // index
        for (j = 0; j < fn; j++) {

            sprintf(file1, "%s%d/HA/HAL%d00%d.bmp", RECONSTRUCTED_DIR, JUMP, i, j);

            // finger
            for (k = 0; k < f; k++) {

                // index
                for (l = 0; l < fn; l++) {

                    sprintf(file2, "%s%d/HA/HAL%d00%d.bmp", RECONSTRUCTED_DIR, JUMP, k, l);
                    printf("%s\n%s\n\n", file1, file2);

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
//                    printf("Extract FD1: %d\n", GMFAPI_Extraction(pixel_image_1, pFD_1));

                    unsigned char *pFD_2 = calloc(FEATURE_DATA_SIZE, sizeof(char));
                    GMFAPI_Extraction(pixel_image_2, pFD_2);
//                    printf("Extract FD2: %d\n", GMFAPI_Extraction(pixel_image_2, pFD_2));

                    if (i == k) genuine += GMFAPI_Matching(pFD_1, pFD_2);
                    else impostor += GMFAPI_Matching(pFD_1, pFD_2);

//                    printf("FD1 vs FD2: %d\n", GMFAPI_Matching(pFD_1, pFD_2));

                    fclose(bmp_ptr_1);
                    fclose(bmp_ptr_2);

                }
            }
        }
    }

    printf("Genuine: %d\n", genuine);
    printf("Impostor: %d\n", impostor);

    return 0;

}
