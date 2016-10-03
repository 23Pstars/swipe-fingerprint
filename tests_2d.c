#include <stdio.h>
#include <stdlib.h>

#include "correlation_algorithm_2d.h"


int main() {

    unsigned char source[CORR_HEIGHT_SIZE][CORR_WIDTH_SIZE] = {{2, 5, 5, 5, 3},
                                                               {2, 8, 6, 5, 6}},
            target[CORR_HEIGHT_SIZE][CORR_WIDTH_SIZE] = {{2, 7, 5, 8, 6},
                                                         {1, 7, 4, 2, 7}};

    unsigned char **source_ptr = malloc(sizeof(unsigned char *) * CORR_HEIGHT_SIZE);
    unsigned char **target_ptr = malloc(sizeof(unsigned char *) * CORR_HEIGHT_SIZE);

    for (int i = 0; i < CORR_HEIGHT_SIZE; i++) {
        *(source_ptr + i) = malloc(sizeof(unsigned char) * CORR_WIDTH_SIZE);
        *(source_ptr + i) = source[i];
        *(target_ptr + i) = malloc(sizeof(unsigned char) * CORR_WIDTH_SIZE);
        *(target_ptr + i) = target[i];
    }

    printf("SAD: %d\n", SAD_2d(source_ptr, target_ptr, 0, 0));
    printf("ZSAD: %f\n", ZSAD_2d(source_ptr, target_ptr, 0, 0));
    printf("SSD: %d\n", SSD_2d(source_ptr, target_ptr, 0, 0));
    printf("ZSSD: %f\n", ZSSD_2d(source_ptr, target_ptr, 0, 0));

    return 0;
}