#include <stdio.h>
#include <stdlib.h>

#include "correlation_algorithm.h"

#define     CORR_WIDTH_SIZE       5
#define     CORR_HEIGHT_SIZE      3

int main() {

    u_int8_t source[CORR_HEIGHT_SIZE * CORR_WIDTH_SIZE] = {2, 5, 5, 5, 3, 2, 8, 6, 5, 6, 2, 5, 5, 5, 3},
            target[CORR_HEIGHT_SIZE * CORR_WIDTH_SIZE] = {2, 7, 5, 8, 6, 1, 7, 4, 2, 7, 2, 7, 5, 8, 6};

    printf("SAD: %d\n", SAD(source, target, CORR_HEIGHT_SIZE, CORR_WIDTH_SIZE, 0, 0));
    printf("ZSAD: %f\n", ZSAD(source, target, CORR_HEIGHT_SIZE, CORR_WIDTH_SIZE, 0, 1));

    return 0;
}