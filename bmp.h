/**
 * short --> 2 bytes
 * int --> 4 bytes
 * long --> 8 bytes
 *
 * kadang berlaku juga padding
 * - http://www.c-faq.com/struct/align.html
 */

typedef struct {
    unsigned short type;                        /* identifikasi jenis gambar */
    unsigned int size;                          /* file size in bytes */
    unsigned short reserved1;                   /* tidak digunakan */
    unsigned short reserved2;                   /* tidak digunakan */
    unsigned int offset;                        /* offset ke data gambar */
} FILEHEADER;

typedef struct {
    unsigned int size;                          /* ukuran infoheader */
    int width, height;                          /* lebar dan tinggi */
    unsigned short planes;                      /* jumlah color planes */
    unsigned short bits;                        /* bits tiap pixel */
    unsigned int compression;                   /* tipe kompresi */
    unsigned int imagesize;                     /* ukuran data gambar */
    int xresolution, yresolution;               /* pizel per meter */
    unsigned int ncolours;                      /* jumlah warna */
    unsigned int importantcolours;              /* warna penting */
} INFOHEADER;

typedef struct {
    unsigned char r, g, b, junk;                /* terkumpul dalam 1 byte */
} COLOURINDEX;

