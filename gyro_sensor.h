#ifndef HEADER_GYRO
#define HEADER_GYRO

int gyro_on_off(XIicPs *Iic, u8 on_off);

int lsm6dsl_gyro_raw_get(XIicPs *Iic, u8* buff);

float lsm6dsl_from_fs2000dps_to_mdps(s16 lsb);

#endif /* HEADER_GYRO */