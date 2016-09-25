/**
 * short --> 2 bytes
 * int --> 4 bytes
 * long --> 8 bytes
 */

typedef struct {
    unsigned short int type;                    /* magic identifier */
    unsigned int size;                          /* file size in bytes */
    unsigned short int reserved1, reserved2;
    unsigned int offset;                        /* offset to image data */
} FILEHEADER;

typedef struct {
    unsigned int size;                          /* header size in bytes */
    int width, height;                          /* width and height of image */
    unsigned short int planes;                  /* number of color planes */
    unsigned short int bits;                    /* bits per pixel */
    unsigned int compression;                   /* compression type */
    unsigned int imagesize;                     /* image size in bytes */
    int xresolution, yresolution;               /* pizel per meter */
    unsigned int ncolours;                      /* number of colours */
    unsigned int importantcolours;              /* important colours */
} INFOHEADER;

typedef struct {
    unsigned char r, g, b, junk;
} COLOURINDEX;

