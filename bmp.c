#include <stdlib.h>
#include <printf.h>

#include "bmp.h"
#include "config.h"

/**
 * buka buffer BMP
 *
 * @return bmp_ptr
 */
FILE *open_bmp() {

    /**
     * buffer BMP
     */
    FILE *bmp_ptr;

    /**
     * cek apakah berhasil akses file BMP
     */
    if ((bmp_ptr = fopen(BMP_SOURCE, "r")) == NULL) {
        printf("Failed to open %s\n", BMP_SOURCE);
        exit(-1);
    }

    return bmp_ptr;
}

/**
 * baca file BMP
 *
 * @param fileheader
 * @param infoheader
 * @param colourindex
 * @param pixel_image
 */
void read_bmp_header(FILEHEADER *fileheader, INFOHEADER *infoheader, COLOURINDEX *colourindex) {

    FILE *bmp_source_ptr = open_bmp();

    /**
     * baca file header
     */
    fread(&fileheader->type, 2, 1, bmp_source_ptr);
    fread(&fileheader->size, 4, 1, bmp_source_ptr);
    fread(&fileheader->reserved1, 2, 1, bmp_source_ptr);
    fread(&fileheader->reserved2, 2, 1, bmp_source_ptr);
    fread(&fileheader->offset, 4, 1, bmp_source_ptr);

    /**
     * deteksi jenis BMP
     */
    if (fileheader->type != 0x4d42) {
        printf("File is not BMP (%x)\n", fileheader->type);
        exit(-1);
    }

    /**
     * baca info header
     */
    fread(infoheader, sizeof(INFOHEADER), 1, bmp_source_ptr);

    /**
     * deteksi jenis BIT
     */
    if (infoheader->bits != BMP_INFO_BIT) {
        printf("Only support BMP 8 bits\n");
        exit(-1);
    }

    /**
     * color index (opsional)
     */
    fread(colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_source_ptr);

    /**
     * close the file pointer
     */
    fclose(bmp_source_ptr);

}

/**
 * baca data pixel BMP
 *
 * @param pixel_image
 * @param imagesize
 * @param offset
 */
void read_bmp_pixel_image(u_int8_t *pixel_image, u_int32_t imagesize, u_int32_t offset) {

    FILE *bmp_source_ptr = open_bmp();

    /**
     * geser ke data utama
     */
    fseek(bmp_source_ptr, offset, SEEK_SET);

    /**
     * baca pixel data
     */
    fread(pixel_image, 1, imagesize, bmp_source_ptr);

    fclose(bmp_source_ptr);
}

/**
 * tulis file BMP
 *
 * @param fileheader
 * @param infoheader
 * @param colourindex
 * @param pixel_image
 */
void write_bmp(FILEHEADER *fileheader, INFOHEADER *infoheader, COLOURINDEX *colourindex, u_int8_t *pixel_image) {

    FILE *bmp_target_ptr;

    if ((bmp_target_ptr = fopen(BMP_TARGET, "wb")) == NULL) {
        printf("Failed to open %s", BMP_TARGET);
    }

    /* file header */
    fwrite(&fileheader->type, 2, 1, bmp_target_ptr);
    fwrite(&fileheader->size, 4, 1, bmp_target_ptr);
    fwrite(&fileheader->reserved1, 2, 1, bmp_target_ptr);
    fwrite(&fileheader->reserved2, 2, 1, bmp_target_ptr);
    fwrite(&fileheader->offset, 4, 1, bmp_target_ptr);

    /* info header */
    fwrite(infoheader, sizeof(INFOHEADER), 1, bmp_target_ptr);

    /* color palette */
    fwrite(colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_target_ptr);

    fseek(bmp_target_ptr, fileheader->offset, SEEK_SET);

    /**
     * Customize with expected height of image
     *
     * @todo
     * modify height of generated image
     */
//    infoheader.height = BMP_GENERATED_HEIGHT;
//    infoheader.imagesize = (u_int32_t) (infoheader.width * infoheader.height);
//    printf("width: %d\nheight: %d\nimagesize: %d\n", infoheader.width, infoheader.height, infoheader.imagesize);

    /**
     * Write image data
     */
    fwrite(pixel_image, 1, infoheader->imagesize, bmp_target_ptr);

    fclose(bmp_target_ptr);

}

/**
 * reverse order tiap n-pixel (max sampling 255 pixel)
 *
 * @param source
 * @param target
 * @param height
 * @param width
 * @param sampling_size
 */
void block_reverse(u_int8_t *source, u_int8_t *target, int32_t height, int32_t width, u_int8_t sampling_size) {

    u_int16_t i, j, k, rate = (u_int16_t) height / sampling_size;

    /* segments */
    for (i = 0; i < rate; i++)

        /* width in sampling (block) */
        for (j = 0; j < sampling_size; j++)

            /* element (column) of rows */
            for (k = 0; k < width; k++)

                target[(i * sampling_size * width) + (j * width) + k] = source[
                        ((height - (sampling_size * (i + 1))) * width) + (j * width) + k];

}

/**
 * merge suatu block (source) kedalam suatu data (target)
 *
 * @param source, block yang akan di-merge
 * @param target, data utuh tujuan merge
 * @param height, tinggi dari block yang akan di-merge (source)
 * @param width, lebar dari block yang akan di-merge (source)
 * @param offset, offset untuk tiap block target didalam target
 * @param h_shift, pergeseran (vertical) source dari target
 * @param w_shift, pergeseran (horizontal) source dari target
 * @param default_w_value, default value untuk pergeseran horizontal
 */
void block_merge(u_int8_t *source, u_int8_t *target, u_int8_t height, u_int8_t width, int16_t offset, int16_t h_shift,
                 int16_t w_shift, u_int8_t default_w_value) {

    u_int16_t i, j;

//    printf("\n\noffset: %d, h_shift: %d, w_shift: %d\n", offset, h_shift, w_shift);

    /**
     * looping untuk setiap row setinggi height dari source
     */
    for (i = 0; i < height; i++) {

        /**
         * looping untuk setiap cell sepanjang width dari source
         */
        for (j = 0; j < width; j++) {

            if (j + w_shift < 0) {                          /** geser kiri */
//                printf("h;%d w;%d %d:%d\t\t", i, j, *(target + ((offset + i + h_shift + 1) * width) + j + w_shift),
//                       default_w_value);
//                target[((offset + i + h_shift + 1) * width) + j + w_shift] = default_w_value;
                *(target + ((offset + i + h_shift + 1) * width) + j + w_shift) = default_w_value;
            } else if (j + w_shift > width - 1) {           /** geser kanan */
//                printf("h;%d w;%d %d:%d\t\t", i, j, *(target + ((offset + i + h_shift - 1) * width) + j + w_shift),
//                       default_w_value);
//                target[((offset + i + h_shift - 1) * width) + j + w_shift] = default_w_value;
                *(target + ((offset + i + h_shift - 1) * width) + j + w_shift) = default_w_value;
            } else {
//                printf("h:%d w:%d %d:%d\t\t", i, j,
//                       *(target + ((offset + i + h_shift) * width) + j + w_shift), *(source + (i * width) + j));
//                target[((offset + i + h_shift) * width) + j + w_shift] = source[(i * width) + j];
                *(target + ((offset + i + h_shift) * width) + j + w_shift) = *(source + (i * width) + j);
            }
        }
//        printf("\n");
    }

}

/**
 * potong array menjadi bagian yang diinginkan
 *
 * @param source, data utuh
 * @param height, tinggi total data utuh
 * @param width, lebar total data utuh
 * @param h_size, tinggi yang diinginkan untuk dipotong
 * @param w_size, lebar yang diinginkan untuk dipotong
 * @param h_offset, geser vertical
 * @param w_offset, geser horizontal
 * @return
 */
u_int8_t *
block_slice(u_int8_t *source, int32_t height, int32_t width, u_int16_t h_size, u_int16_t w_size, u_int16_t h_offset,
            u_int16_t w_offset) {

    u_int16_t i, j;
    u_int8_t *block = malloc(sizeof(u_int8_t) * h_size * w_size);

    for (i = 0; i < h_size; i++)

        for (j = 0; j < w_size; j++)

            block[(i * w_size) + j] = (u_int8_t) (h_offset + i > height - 1 || w_offset + j > width - 1 ? 0 : source[
                    ((i + h_offset) * width) + j + w_offset]);

    return block;
}