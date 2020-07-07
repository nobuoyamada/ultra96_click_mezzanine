#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include "xspips.h"

#include "define.h"
#include "spi.h"

int SPI_Init(XSpiPs *SpiI)
{
    int Status;
    XSpiPs_Config *SpiConfig;

    SpiConfig = XSpiPs_LookupConfig(SPI_DEVICE_ID);
    if(SpiConfig == NULL){
        printf("Error: XSpiPs_LookupConfig()\n");
        return XST_FAILURE;
    }    

    Status = XSpiPs_CfgInitialize(SpiI, SpiConfig, SpiConfig->BaseAddress);
    if(Status != XST_SUCCESS){
        printf("Error: XSpiPs_CfgInitialize()\n");
        return XST_FAILURE;
    }

	Status = XSpiPs_SelfTest(SpiI);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XSpiPs_SetOptions(SpiI, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION);
    //XSpiPs_SetOptions(SpiI, XSPIPS_MASTER_OPTION);
	XSpiPs_SetClkPrescaler(SpiI, XSPIPS_CLK_PRESCALE_128);

    printf("SPI configuration done.\n");

    return XST_SUCCESS;

}

int MCP23S17_Write(XSpiPs *SpiI, uint8_t addr, uint8_t tbyte)
{
    int Status;

    uint8_t buff[3];
    buff[0] = MCP23S17_OPCODE << 1;
    buff[1] = addr;
    buff[2] = tbyte;

    Status = XSpiPs_PolledTransfer(SpiI, buff, NULL, sizeof(buff));
	if (Status != XST_SUCCESS) {
		printf("Error: MCP23S17_Write()\n");
	} 

    return Status;
}

int MCP23S17_Read(XSpiPs *SpiI, uint8_t* rbuff, uint8_t addr, u32 length)
{
    int Status;
    uint8_t buff_temp[2];
    buff_temp[0] = MCP23S17_OPCODE << 1 | 0x01;
    buff_temp[1] = addr;

    Status = XSpiPs_PolledTransfer(SpiI, buff_temp, rbuff, sizeof(buff_temp)+length);
	if (Status != XST_SUCCESS) {
		printf("Error: MCP23S17_Read()\n");
	}

    return Status;
}
