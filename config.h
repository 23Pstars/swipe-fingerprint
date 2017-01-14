/**
 * parameter file BMP
 */
#define     BMP_SOURCE              "/Users/zaf/Google Drive/Kuliah/Universitas Gadjah Mada (S2)/thesis/v2/apps/BMP/AGUS_02_000.bmp"
#define     BMP_TARGET              "/Users/zaf/Google Drive/Kuliah/Universitas Gadjah Mada (S2)/thesis/v2/apps/BMP/GENERATED_BMP.bmp"

#define     BMP_REVERSED_SOURCE     "/Users/zaf/Google Drive/Kuliah/Universitas Gadjah Mada (S2)/thesis/v2/apps/BMP/REVERSED_SOURCE_BMP.bmp"

#define     BMP_INFO_BIT                8
#define     BMP_COLOUR_INDEX_LENGTH     256
#define     BMP_GENERATED_HEIGHT        400
//#define     BMP_SOURCE_WIDTH            128
//#define     BMP_SOURCE_HEIGHT           4000

/**
 * tinggi (h_size) tiap block
 */
#define     SAMPLING_SIZE               8

/**
 * offset tiap pencocokan apakah geser kiri,
 * kanan, atas, atau bawah
 */
#define     HEIGHT_RANGE_OFFSET         8
#define     WIDTH_RANGE_OFFSET          0

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