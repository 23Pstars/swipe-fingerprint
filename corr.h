void reconstruct(unsigned char *, unsigned char *);

void calculate_xy(unsigned char *, unsigned char *, short *, short *);

unsigned int diff_xy(unsigned char *, unsigned char *, unsigned short, unsigned short, char, char, char);

unsigned int apply_algo(unsigned char, unsigned char);

unsigned int SAD_algo(unsigned char, unsigned char);

unsigned int SSD_algo(unsigned char, unsigned char);

//unsigned int SAD(unsigned char *, unsigned char *, unsigned short, unsigned short, char, char);
//
//unsigned int SSD(unsigned char *, unsigned char *, unsigned short, unsigned short, char, char);
//
//float NCC(unsigned char *, unsigned char *, unsigned short, unsigned short, char, char);
//
//float ZSAD(unsigned char *, unsigned char *, unsigned short, unsigned short, char, char);
//
//float ZSSD(unsigned char *, unsigned char *, unsigned short, unsigned short, char, char);
//
//float ZNCC(unsigned char *, unsigned char *, unsigned short, unsigned short, char, char);
//
//float mean(unsigned char *, unsigned short, unsigned short);
//
//float sum_squared(unsigned char *, unsigned short, unsigned short);
//
//float sum_mean_squared(unsigned char *, unsigned short, unsigned short);
