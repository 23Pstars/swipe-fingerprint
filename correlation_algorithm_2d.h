//
// Created by Ahmad Zafrullah on 9/27/16.
//

#define     CORR_WIDTH_SIZE       5
#define     CORR_HEIGHT_SIZE      2

int SAD_2d(unsigned char **, unsigned char **, int, int);

float ZSAD_2d(unsigned char **, unsigned char **, int, int);

int SSD_2d(unsigned char **, unsigned char **, int, int);

float ZSSD_2d(unsigned char **, unsigned char **, int, int);

float mean_2d(unsigned char **);