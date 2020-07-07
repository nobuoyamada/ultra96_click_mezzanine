#ifndef HEADER_SPI
#define HEADER_SPI

int SPI_Init(XSpiPs *SpiI);

int MCP23S17_Write(XSpiPs *SpiI, uint8_t addr, uint8_t tbyte);

int MCP23S17_Read(XSpiPs *SpiI, uint8_t* rbuff, uint8_t addr, u32 length);

#endif