/**
 * file buffer
 */

#ifdef __APPLE__
#define     BMP_INPUT_DIR               "/Users/zaf/thesis-apps/Sample-Data/RawImageDB-Zaf/"
#define     BMP_OUTPUT_DIR              "/Users/zaf/thesis-apps/swipe-fingerprint/assets/"
#elif __linux__
#define     BMP_INPUT_DIR               "/home/zaf/thesis-apps/Sample-Data/RawImageDB-Zaf/"
#define     BMP_OUTPUT_DIR              "/home/zaf/thesis-apps/swipe-fingerprint/assets/"
#endif

/**
 * dimensi
 */
#define     BMP_INPUT_WIDTH             128
#define     BMP_INPUT_HEIGHT            4000
#define     BMP_OUTPUT_WIDTH            128
#define     BMP_OUTPUT_WIDTH2           126
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

/**
 * offset tiap pencocokan apakah geser kiri,
 * kanan, atas, atau bawah
 */
#define     HEIGHT_RANGE_OFFSET         7
#define     WIDTH_RANGE_OFFSET          0
#define     JUMPING_RANGE               2
#define     JUMPING_COUNT               10

/**
 * jika treshold sudah terpenuhi program tidak perlu
 * lagi untuk melakukan looping
 */
#define     DIFF_THRESHOLD_MIN          0

/**
 * nilai default
 */
#define     DEFAULT_OVERLAP_VALUE       0

/**
 * 1. SAD
 * 2. SSD
 * 3. NCC
 */
#define     USE_ALGO                    1
#define     USE_JUMPING                 1