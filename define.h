#ifndef HEADER_DEFINE
#define HEADER_DEFINE

// I2C parameters
#define IIC_SCLK_RATE       100000  // clock 100KHz
#define TCA9548A_ADDRESS      0x75    // 7bit address
#define LSM6DSL_ADDRESS		0x6A
#define IIC_DEVICE_ID       XPAR_XIICPS_0_DEVICE_ID
#define SPI_DEVICE_ID       XPAR_XSPIPS_0_DEVICE_ID
#define MCP23S17_OPCODE     0x20    // 7bit address
#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID

#define CH0 0
#define CH1 1
#define CH2 2
#define CH3 3
#define CH4 4
#define CH5 5
#define CH6 6
#define CH7 7

#define CTRL1_XL  0x10
#define CTRL2_G   0x11
#define OUTX_L_G  0x22
#define OUTX_L_XL 0x28

#define ACC_ON    0x01
#define ACC_OFF   0x00

#define GYRO_ON    0x01
#define GYRO_OFF   0x00

#endif
