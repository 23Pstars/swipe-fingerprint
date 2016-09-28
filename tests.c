#include <stdio.h>
#include <stdlib.h>

#include "correlation_algorithm.h"


int main(int argc, char *argv[]) {

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

    printf("SAD: %d\n", SAD(source_ptr, target_ptr, 1, 0));
    printf("ZSAD: %f\n", ZSAD(source_ptr, target_ptr, 0, 0));

    return 0;
}