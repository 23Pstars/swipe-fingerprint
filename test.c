/**
 * testing purpose
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#include "corr.h"
#include "bmp.h"

#define     BLOCK_A         "/Users/zaf/thesis-apps/swipe-fingerprint/assets/blocks/block-0.bmp"
#define     BLOCK_B         "/Users/zaf/thesis-apps/swipe-fingerprint/assets/blocks/block-8.bmp"
#define     BLOCK_c         "/Users/zaf/thesis-apps/swipe-fingerprint/assets/blocks/block-16.bmp"
#define     BLOCK_HEIGHT    8
#define     BLOCK_WIDTH     128
#define     BLOCK_SIZE      BLOCK_HEIGHT * BLOCK_WIDTH
#define     VECTOR_WIDTH    128

#define     n               4


int main() {

    FILE *bmp_ptr;
    unsigned int offset;
    unsigned char *block_a = calloc(BLOCK_SIZE, sizeof(unsigned char)),
            *block_b = calloc(BLOCK_SIZE, sizeof(unsigned char)),
            *block_c = calloc(BLOCK_SIZE, sizeof(unsigned char)),
            *block_abc = calloc(BLOCK_SIZE * 3, sizeof(unsigned char));

    bmp_ptr = open_bmp_file(BLOCK_A, "r");
    fseek(bmp_ptr, 1078, SEEK_SET);
    fread(block_a, 1, BLOCK_SIZE, bmp_ptr);

//    fseek(bmp_ptr, 1078, SEEK_SET);
//    memset(block_b, 0, BLOCK_WIDTH);
//    fread(block_b + BLOCK_WIDTH, 1, BLOCK_SIZE - BLOCK_WIDTH, bmp_ptr);


//    bmp_ptr = open_bmp_file(BLOCK_B, "r");
//    fseek(bmp_ptr, 1078, SEEK_SET);
//    fread(block_b, 1, BLOCK_SIZE, bmp_ptr);
//    fread(vector_b, 1, VECTOR_WIDTH, bmp_ptr);

//    memset(block_a, 1, BLOCK_SIZE);
//    memset(block_b, 2, BLOCK_SIZE);
//    memset(block_c, 3, BLOCK_SIZE);
//
    for (unsigned short h = 0; h < BLOCK_SIZE; h++)
        printf("%d\t%s", *(block_a + h), ((h + 1) % BLOCK_WIDTH == 0) ? "\n" : "");

    printf("\n");

    for (unsigned short h = 0; h < BLOCK_SIZE; h++)
        printf("%d\t%s", *(block_b + h), ((h + 1) % BLOCK_WIDTH == 0) ? "\n" : "");

    printf("\n");
//
//    for (unsigned short h = 0; h < BLOCK_SIZE; h++)
//        printf("%d\t%s", *(block_c + h), ((h + 1) % BLOCK_WIDTH == 0) ? "\n" : "");
//
//    printf("\n");
//
//    block_merge(block_abc, block_a, BLOCK_HEIGHT, BLOCK_WIDTH, 1, 3, 0, 9);
//
//    for (unsigned short h = 0; h < BLOCK_SIZE * 3; h++)
//        printf("%d\t%s", *(block_abc + h), ((h + 1) % BLOCK_WIDTH == 0) ? "\n" : "");
//
//    unsigned int diff = 0xffffffff, diff_temp;
//
//    char h_range = 7, w_range = 8, h_min = -h_range, w_min = -w_range;
//    for (char hs = -h_range; hs <= h_range; hs++)
//        for (char ws = -w_range; ws <= w_range; ws++) {
//            diff_temp = diff_xy(block_a, block_b, BLOCK_HEIGHT, BLOCK_WIDTH, hs, ws, false);
//            printf("SAD(hs %d, ws %d): %d\n", hs, ws, diff_temp);
//            if (diff_temp < diff) {
//                diff = diff_temp;
//                h_min = hs;
//                w_min = ws;
//            }
//        }

//    char w_range = 1, w_min = -w_range;
//    for (char ws = -w_range; ws <= w_range; ws++) {
//        diff_temp = diff_xy_vector(block_a, block_b, VECTOR_WIDTH, ws, false);
//        printf("SAD(ws %d): %d\n", ws, diff_temp);
//        if (diff_temp < diff) {
//            diff = diff_temp;
//            w_min = ws;
//        }
//    }
//
//    printf("w_min: %d\n", w_min);

//    diff_temp = diff_xy_vector(vector_a, vector_b, VECTOR_WIDTH, 1, false);

//    unsigned char a[n * n] = {
//            1, 1, 1, 1,
//            2, 2, 2, 2,
//            3, 3, 3, 3,
//            4, 4, 4, 4
//    };
//
//    unsigned char b[n * n * n] = {
//            5, 5, 5, 5,
//            1, 1, 1, 1,
//            2, 2, 2, 2,
//            3, 3, 3, 3,
//            4, 4, 4, 4,
//            2, 2, 2, 2,
//            3, 3, 3, 3,
//            4, 4, 4, 4
//    };

//    unsigned char c[n * n] = {
//            1, 1, 1, 1,
//            2, 2, 2, 2,
//            3, 3, 3, 3,
//            4, 4, 4, 4
//    };
//
//    unsigned char d[n * n] = {
//            2, 2, 2, 2,
//            3, 3, 3, 3,
//            4, 4, 4, 4,
//            5, 5, 5, 5
//    };
//

//    unsigned char e_size = n * n, ef_size = (unsigned char) (e_size * 3);
//
//    unsigned char e[n * n] = {
//            0, 1, 2, 3,
//            4, 5, 6, 7,
//            8, 9, 10, 11,
//            12, 13, 14, 15
//    };
//
//    unsigned char f[n * n] = {
//            4, 5, 6, 7,
//            8, 9, 10, 11,
//            12, 13, 14, 15,
//            16, 17, 18, 19
//    };
//
//    unsigned char g[n * n] = {
//            8, 9, 10, 11,
//            12, 13, 14, 15,
//            16, 17, 18, 19,
//            20, 21, 22, 23
//    };

//    for (unsigned char i = 0; i < e_size; i++)
//        printf("%d\t%s", *(e + i), ((i + 1) % n == 0 ? "\n" : ""));
//
//    printf("\n");
//
//
//    unsigned int diff = 66585600, diff_temp;
//    char h_min = -n, w_min = -n;
//    for (char hs = -n + 1; hs < n; hs++)
//        for (char ws = -n + 1; ws < n; ws++) {
//            diff_temp = diff_xy(e, f, n, n, hs, ws, false);
//            printf("SAD(hs %d, ws %d): %d\n", hs, ws, diff_temp);
//            if (diff_temp < diff) {
//                diff = diff_temp;
//                h_min = hs;
//                w_min = ws;
//            }
//        }
//
//    printf("h_min: %d, w_min: %d\n", h_min, w_min);

//    unsigned char ef[ef_size];
//    memset(ef, 0, ef_size);
//    memcpy(ef, e, e_size);
//    block_merge(ef, f, n, n, 1, 0, 0, 99);
//    block_merge(ef, g, n, n, 2, 0, 0, 99);
//
//    for (unsigned char i = 0; i < ef_size; i++)
//        printf("%d\t%s", *(ef + i), ((i + 1) % n == 0 ? "\n" : ""));

}