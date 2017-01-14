void reconstruct(u_int8_t *, u_int8_t *, int32_t, int32_t);

u_int32_t SAD(u_int8_t *, u_int8_t *, u_int16_t, u_int16_t, int8_t, int8_t);

u_int32_t SSD(u_int8_t *, u_int8_t *, u_int16_t, u_int16_t, int8_t, int8_t);

float NCC(u_int8_t *, u_int8_t *, u_int16_t, u_int16_t, int8_t, int8_t);

float ZSAD(u_int8_t *, u_int8_t *, u_int16_t, u_int16_t, int8_t, int8_t);

float ZSSD(u_int8_t *, u_int8_t *, u_int16_t, u_int16_t, int8_t, int8_t);

float ZNCC(u_int8_t *, u_int8_t *, u_int16_t, u_int16_t, int8_t, int8_t);

float mean(u_int8_t *, u_int16_t, uint16_t);

float sum_squared(u_int8_t *, u_int16_t, uint16_t);

float sum_mean_squared(u_int8_t *, u_int16_t, uint16_t);
