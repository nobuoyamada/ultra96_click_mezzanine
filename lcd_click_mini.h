#ifndef HEADER_LCD
#define HEADER_LCD

#define IODIRB 0x01
#define IOCON  0x0B
#define GPIOB  0x13

#define DISPON 0x01
#define DISPOFF 0x00
#define CURSOR_ON 0x01
#define CURSOR_OFF 0x00
#define BLINK_ON 0x01
#define BLINK_OFF 0x00

#define DL_8BIT 0x01
#define DL_4BIT 0x00
#define N_2LINE 0x01
#define N_1LINE 0x00
#define F_5x11  0x01
#define F_5x8   0x00

#define SCR_SHIFT 0x01
#define NO_SCR_SHIFT 0x00
#define DIR_LEFT 0x01
#define DIR_RIGHT 0x00

int LcdPinInit(XSpiPs *SpiI);

int LcdCommandWrite(XSpiPs *SpiI, uint8_t data);
int LcdDataWrite(XSpiPs *SpiI, uint8_t data);

int Write4bit(XSpiPs *SpiI, uint8_t data);

int DisplayOnOff(XSpiPs *SpiI, uint8_t disp, uint8_t cursor, uint8_t blink);

int FunctionSet(XSpiPs *SpiI, uint8_t dl, uint8_t n, uint8_t f);

int EntryMode(XSpiPs *SpiI, uint8_t id, uint8_t s);

int ClearDisplay(XSpiPs *SpiI);

int MCP4161_setup(XSpiPs *SpiI, uint8_t contrast);

#endif