/**
 * file buffer
 */
#define     BMP_INPUT                   "/Users/zaf/Google Drive/Kuliah/Universitas Gadjah Mada (S2)/thesis/v2/apps/BMP/assets/8RUL2001.bmp"
#define     BMP_OUTPUT                  "/Users/zaf/Google Drive/Kuliah/Universitas Gadjah Mada (S2)/thesis/v2/apps/BMP/assets/ReconstructedBMP.bmp"

/**
 * dimensi
 */
#define     BMP_INPUT_WIDTH             128
#define     BMP_INPUT_HEIGHT            4000
#define     BMP_OUTPUT_WIDTH            128
#define     BMP_OUTPUT_HEIGHT           400

/**
 * size
 */
#define     BMP_INPUT_SIZE              BMP_INPUT_WIDTH * BMP_INPUT_HEIGHT
#define     BMP_OUTPUT_SIZE             BMP_OUTPUT_WIDTH * BMP_OUTPUT_HEIGHT

/**
 * tinggi (h_size) tiap block
 */
#define     BLOCK_HEIGHT                8
#define     BLOCK_SIZE                  BLOCK_HEIGHT * BMP_INPUT_WIDTH
#define     BLOCK_COUNT                 BMP_INPUT_SIZE / BLOCK_HEIGHT

/**
 * offset tiap pencocokan apakah geser kiri,
 * kanan, atas, atau bawah
 */
#define     HEIGHT_RANGE_OFFSET         7
#define     WIDTH_RANGE_OFFSET          8

/**
 * jika treshold sudah terpenuhi program tidak perlu
 * lagi untuk melakukan looping
 */
#define     DIFF_THRESHOLD_MIN          0

/**
 * perlu dibatasi differences maksimal antar block
 * karena bisa jadi block tersebut memang berurutan
 * (tidak overlapping)
 */
#define     DIFF_THRESHOLD_MAX          0