/**
 * testing purpose
 */

#include <stdio.h>
#include <stdlib.h>

#include "corr.h"

#define n 4

int main() {

    unsigned char a[n * n] = {
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4
    };

    unsigned char b[n * n * n] = {
            5, 5, 5, 5,
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            2, 2, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4
    };

    unsigned char c[n * n] = {
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4
    };

    unsigned char d[n * n] = {
            2, 2, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            5, 5, 5, 5
    };

    for (char i = -3; i <= 3; i++)
        printf("SDA(%d): %d\n", i, SAD(a, b, n, n, i, 0));

}