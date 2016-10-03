/**
 * char (int8_t)        --> 1 byte
 * short (int16_t)      --> 2 bytes
 * int (int32_t)        --> 4 bytes
 * long (int64_t)       --> 8 bytes
 *
 * kadang berlaku juga padding
 * - http://www.c-faq.com/struct/align.html
 */

typedef struct {
    u_int16_t type;                             /* identifikasi jenis gambar */
    u_int32_t size;                             /* file size in bytes */
    u_int16_t reserved1;                        /* tidak digunakan */
    u_int16_t reserved2;                        /* tidak digunakan */
    u_int32_t offset;                           /* offset ke data gambar */
} FILEHEADER;

typedef struct {
    u_int32_t size;                             /* ukuran infoheader */
    int32_t width, height;                      /* lebar dan tinggi */
    u_int16_t planes;                           /* jumlah color planes */
    u_int16_t bits;                             /* bits tiap pixel */
    u_int32_t compression;                      /* tipe kompresi */
    u_int32_t imagesize;                        /* ukuran data gambar */
    int32_t xresolution, yresolution;           /* pizel per meter */
    u_int32_t  ncolours;                        /* jumlah warna */
    u_int32_t  importantcolours;                /* warna penting */
} INFOHEADER;

typedef struct {
    u_int16_t r, g, b, junk;                    /* terkumpul dalam 1 byte */
} COLOURINDEX;

void reverse_order(u_int8_t *, u_int8_t *, int32_t, int32_t, uint8_t);