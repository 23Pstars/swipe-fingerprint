/**
 * char     --> 1 byte
 * short    --> 2 bytes
 * int      --> 4 bytes
 * long     --> 8 bytes
 *
 * kadang berlaku juga padding
 * - http://www.c-faq.com/struct/align.html
 */

#define     BMP_INFO_BIT                8
#define     BMP_COLOUR_INDEX_LENGTH     128

typedef struct {
    unsigned short type;        /** identifikasi jenis gambar */
    unsigned int size;          /** file size in bytes */
    unsigned short reserved1;   /** tidak digunakan */
    unsigned short reserved2;   /** tidak digunakan */
    unsigned int offset;        /** offset ke data gambar */
} FILEHEADER;

typedef struct {
    unsigned int size;              /** ukuran infoheader */
    int width, height;              /** lebar dan tinggi */
    unsigned short planes;          /** jumlah color planes */
    unsigned short bits;            /** bits tiap pixel */
    unsigned int compression;       /** tipe kompresi */
    unsigned int imagesize;         /** ukuran data gambar */
    int xresolution, yresolution;   /** pizel per meter */
    unsigned int ncolours;          /** jumlah warna */
    unsigned int importantcolours;  /** warna penting */
} INFOHEADER;

typedef struct {
    unsigned short r, g, b, junk;                    /* terkumpul dalam 1 byte */
} COLOURINDEX;

typedef struct {
    FILEHEADER fileheader;
    INFOHEADER infoheader;
    COLOURINDEX colourindex[BMP_COLOUR_INDEX_LENGTH];
} BMPHEADER;

FILE *open_bmp_file(char *, char *);

void load_bmp(BMPHEADER *, unsigned char *);

void read_bmp_header(FILEHEADER *, INFOHEADER *, COLOURINDEX *);

void read_bmp_pixel_image(unsigned char *, unsigned int, unsigned int);

void save_bmp(BMPHEADER *, unsigned char *);

void write_bmp(FILEHEADER *, INFOHEADER *, COLOURINDEX *, unsigned char *);

void block_merge(unsigned char *, unsigned char *, unsigned char, unsigned char, short, short, short, unsigned char);

void block_slice(unsigned char *, unsigned char *, unsigned short, unsigned short);

void block_reverse(unsigned char *, unsigned char *);