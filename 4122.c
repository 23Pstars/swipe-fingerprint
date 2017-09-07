/**
 * Ahmad Zafrullah
 * https://zaf.web.id
 *
 * Universitas Gadjah Mada
 *
 * Reconstruct overlap image in swipe fingerprint
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

    char file1[256];
    sprintf(file1, "%s/times2/range_%d.csv", BMP_OUTPUT_DIR, JUMPING_RANGE);
    FILE *times_ptr = fopen(file1, "a");

    BMPHEADER bmpheader;
    unsigned char
            *pixel_image = malloc(BMP_INPUT_SIZE * sizeof(unsigned char)),
            *pixel_image_reversed = malloc(BMP_INPUT_SIZE * sizeof(unsigned char)),
            *pixel_image_generate = malloc(BMP_OUTPUT_SIZE * sizeof(unsigned char));

    char bmp_input[250], bmp_output[250];
    unsigned char i, j;
    for (i = 0; i <= 5; i++) {
        for (j = 0; j <= 5; j++) {

            sprintf(bmp_input, "%sHA/HAL%d00%d.bmp", BMP_INPUT_DIR, i, j);
            sprintf(bmp_output, "%sHA/HAL%d00%d.bmp", BMP_OUTPUT_DIR, i, j);

            memset(pixel_image_generate, DEFAULT_OVERLAP_VALUE, BMP_OUTPUT_SIZE);

            load_bmp(bmp_input, &bmpheader, pixel_image);

            block_reverse(pixel_image, pixel_image_reversed);

            reconstruct(pixel_image_reversed, pixel_image_generate);

            save_bmp(bmp_output, &bmpheader, pixel_image_generate);
        }
    }

//    free(pixel_image);
//    free(pixel_image_reversed);
//    free(pixel_image_generate);

    /**
     * execution time
     */
    printf("%.10f\n", (double) (clock() - time_start) / CLOCKS_PER_SEC);
//    fprintf(times_ptr, "%d,%f\n", JUMPING_COUNT, (double) (clock() - time_start) / CLOCKS_PER_SEC);

    return 0;
}
