#include <stdlib.h>
#include <printf.h>
#include <memory.h>

#include "bmp.h"
#include "config.h"

/**
 * buka buffer BMP
 *
 * @return bmp_ptr
 */
FILE *open_bmp_file(char *file_path, char *mode) {

    /**
     * buffer BMP
     */
    FILE *bmp_ptr;

    /**
     * cek apakah berhasil akses file BMP
     */
    if ((bmp_ptr = fopen(file_path, mode)) == NULL) {
        printf("Failed to open %s\n", file_path);
        exit(-1);
    }

    return bmp_ptr;
}

/**
 * load file BMP
 *
 * @param bmpheader
 */
void load_bmp(BMPHEADER *bmpheader, unsigned char *pixel_image) {

    /**
     * baca header
     */
    read_bmp_header(&bmpheader->fileheader, &bmpheader->infoheader, bmpheader->colourindex);

    /**
     * baca data pixel image
     */
    read_bmp_pixel_image(pixel_image, bmpheader->infoheader.imagesize, bmpheader->fileheader.offset);

}

/**
 * baca file header
 * @param fileheader
 * @param infoheader
 * @param colourindex
 */
void read_bmp_header(FILEHEADER *fileheader, INFOHEADER *infoheader, COLOURINDEX *colourindex) {

    FILE *bmp_source_ptr = open_bmp_file(BMP_INPUT, "r");

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
void read_bmp_pixel_image(unsigned char *pixel_image, unsigned int imagesize, unsigned int offset) {

    FILE *bmp_source_ptr = open_bmp_file(BMP_INPUT, "r");

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
 * save image to output
 *
 * @param bmpheader
 * @param pixel_image
 */
void save_bmp(BMPHEADER *bmpheader, unsigned char *pixel_image) {

    /**
     * modif header sesuai ukuran output
     */
    bmpheader->infoheader.height = BMP_OUTPUT_HEIGHT;
    bmpheader->infoheader.imagesize = BMP_OUTPUT_SIZE;

    /**
     * fileheader: 14 bytes
     * infoheader: 40 bytes
     * colourindex: 128 * 8 bytes
     * Total: 1078
     */
    bmpheader->fileheader.size = (unsigned int) (1078 + BMP_OUTPUT_SIZE);

    /**
     * tulis data pixel
     */
    write_bmp(&bmpheader->fileheader, &bmpheader->infoheader, bmpheader->colourindex, pixel_image);
}

/**
 * tulis file BMP
 *
 * @param fileheader
 * @param infoheader
 * @param colourindex
 * @param pixel_image
 */
void write_bmp(FILEHEADER *fileheader, INFOHEADER *infoheader, COLOURINDEX *colourindex, unsigned char *pixel_image) {

    FILE *bmp_target_ptr = open_bmp_file(BMP_OUTPUT, "wb");

    fwrite(&fileheader->type, 2, 1, bmp_target_ptr);
    fwrite(&fileheader->size, 4, 1, bmp_target_ptr);
    fwrite(&fileheader->reserved1, 2, 1, bmp_target_ptr);
    fwrite(&fileheader->reserved2, 2, 1, bmp_target_ptr);
    fwrite(&fileheader->offset, 4, 1, bmp_target_ptr);

    fwrite(infoheader, sizeof(INFOHEADER), 1, bmp_target_ptr);
    fwrite(colourindex, sizeof(COLOURINDEX), BMP_COLOUR_INDEX_LENGTH, bmp_target_ptr);

    fseek(bmp_target_ptr, fileheader->offset, SEEK_SET);
    fwrite(pixel_image, 1, infoheader->imagesize, bmp_target_ptr);

    fclose(bmp_target_ptr);

}

/**
 * merge suatu block (source) kedalam suatu data (target)
 *
 * @param pixel_image_generate, block tujuan merge
 * @param block_buffer, buffer data yang akan di-merge
 * @param block_height, tinggi dari block yang akan di-merge (source)
 * @param block_width, lebar dari block yang akan di-merge (source)
 * @param iterate_offset, offset untuk tiap block target didalam target
 * @param height_shift, pergeseran (vertical) source dari target
 * @param width_shift, pergeseran (horizontal) source dari target
 */
void block_merge(unsigned char *pixel_image_generate, unsigned char *block_buffer, unsigned char block_height,
                 unsigned char block_width, short iterate_offset, short height_shift, short width_shift) {

    /**
     * variabel iterasi
     */
    unsigned short i;

    /**
     * isi block yang akan diisi dengan `default_overlap_value`
     * untuk memudahkan identifikasi pergeseran dx dan dy
     */
    memset(pixel_image_generate + (iterate_offset * block_width), DEFAULT_OVERLAP_VALUE,
           (height_shift + block_height) * block_width);

    if (height_shift >= 0) {

        if (width_shift >= 0) {

            /**
             * height_shift +
             * width_shift +
             */
            for (i = 0; i < block_height; i++) {
                memcpy(pixel_image_generate + ((i + iterate_offset + height_shift) * block_width) + width_shift,
                       block_buffer + (i * block_width),
                       block_width - width_shift);
            }

        } else {

            /**
             * height_shift +
             * width_shift -
             */
            for (i = 0; i < block_height; i++) {
                memcpy(pixel_image_generate + ((i + iterate_offset + height_shift) * block_width),
                       block_buffer + (i * block_width) - width_shift,
                       block_width + width_shift);
            }
        }
    } else {

        if (width_shift >= 0) {

            /**
             * height_shift -
             * width_shift +
             */
            for (i = 0; i < block_height + height_shift; i++) {
                memcpy(pixel_image_generate + ((i + iterate_offset) * block_width) + width_shift,
                       block_buffer + ((i - height_shift) * block_width),
                       block_width - width_shift);
            }

        } else {

            /**
             * height_shift -
             * width_shift -
             */
            for (i = 0; i < block_height + height_shift; i++) {
                memcpy(pixel_image_generate + ((i + iterate_offset) * block_width),
                       block_buffer + ((i - height_shift) * block_width) - width_shift,
                       block_width + width_shift);
            }
        }
    }

}

/**
 * potong array menjadi bagian dengan ukuran BLOCK_SIZE
 *
 * @param pixel_image, data sumber dari pixel gambar
 * @param block_buffer, block hasil slice
 * @param height_offset, offset untuk tinggi pixel gambar
 * @param block_size, ukuran block yang akan dipotong (custom)
 */
void block_slice(unsigned char *pixel_image, unsigned char *block_buffer,
                 unsigned short height_offset, unsigned short block_size) {
    memcpy(block_buffer, pixel_image + (height_offset * BMP_INPUT_WIDTH), block_size);
}

/**
 * reverse tiap row untuk masing-masing bagian block
 *
 * @param pixel_image
 * @param pixel_image_reversed
 */
void block_reverse(unsigned char *pixel_image, unsigned char *pixel_image_reversed) {

    unsigned int i;
    unsigned short j;

    /**
     * iterasi per-block
     */
    for (i = 0; i < BMP_INPUT_SIZE; i += BLOCK_SIZE) {

        /**
         * iterasi per-row untuk setiap block
         */
        for (j = 0; j < BLOCK_SIZE; j += BMP_INPUT_WIDTH) {

            /**
             * lakukan copy per-row
             */
            memcpy(pixel_image_reversed + i + j, pixel_image + i + (BLOCK_SIZE - BMP_INPUT_WIDTH - j), BMP_INPUT_WIDTH);

        }

    }


}

/**
 * @TODO
 * nilai mean dari suatu block
 *
 * @param block_buffer
 * @param height_shift
 * @param width_shift
 * @return
 */
float block_mean(unsigned char *block_buffer, char height_shift, char width_shift) {
    return 0;
}