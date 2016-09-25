//
// Created by Ahmad Zafrullah on 9/19/16.
//

#include <stdio.h>

#include "bmp.h"
#include "data.h"

int main() {

    int i, j;
    HEADER header;
    INFOHEADER infoheader;
    COLOURINDEX colourindex;
    FILE *bmp_source_ptr, *bmp_target_ptr;

    /* open file */
    if( ( bmp_source_ptr = fopen( BMP_SOURCE, "r" ) ) == NULL ) {
        fprintf( stderr, "Failed to open %s\n", BMP_SOURCE );
        return -1;
    }

    fprintf( stderr, "File header size: %zu\n", sizeof( HEADER ) );
    fprintf( stderr, "Short(%zu), Int(%zu), Long(%zu)\n", sizeof(short), sizeof(int), sizeof(long) );

    /* read file header */
    if( fread( &header, sizeof( HEADER ), 1, bmp_source_ptr ) != 1 ) {
        fprintf( stderr, "Failed to read file header\n" );
        return -1;
    }
//    fprintf( stderr, "FILE INFO\nType: %x\nSize: %d\nOffset: %d\n", header.type, header.size, header.offset );



    return 0;
}