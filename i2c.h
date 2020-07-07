#ifndef HEADER_I2C
#define HEADER_I2C

int Init(XIicPs *Iic);

int i2c_write(XIicPs *Iic, u8 command, u16 i2c_adder);

int i2c_read(XIicPs *Iic, u8* buff, u32 len, u16 i2c_adder);

int i2c_switch_on(XIicPs *Iic, u8 channel);

#endif