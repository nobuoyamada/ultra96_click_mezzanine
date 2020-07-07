#ifndef HEADER_ACC
#define HEADER_ACC

int lsm6dsl_read_reg(XIicPs *Iic, u8* buff, u8 address, u32 len);

int lsm6dsl_write_reg(XIicPs *Iic, u8* data, u8 address, u32 len);

int acc_on_off(XIicPs *Iic, u8 on_off);

void lsm6dsl_regdump(XIicPs *Iic);

int lsm6dsl_acc_raw_get(XIicPs *Iic, u8* buff);

float lsm6dsl_from_fs2g_to_mg(s16 lsb);

#endif /* HEADER_ACC */