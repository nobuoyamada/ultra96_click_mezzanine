#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include "xspips.h"

#include "define.h"
#include "spi.h"
#include "lcd_click_mini.h"

int LcdPinInit(XSpiPs *SpiI)
{
    int Status;

    XSpiPs_SetSlaveSelect(SpiI, 0x1);
    
    // init MCP23S17
    Status = MCP23S17_Write(SpiI, IODIRB, 0x00);
    if(Status != XST_SUCCESS){
        printf("Error: Set IODIRB\n");
    }

    Status = MCP23S17_Write(SpiI, GPIOB, 0x00);
    if(Status != XST_SUCCESS){
        printf("Error: Set GPIOB\n");
    }

    // Send 8 bits command
    Write4bit(SpiI, 0x03);
    usleep(5000);

    // Send 8 bits command
    Write4bit(SpiI, 0x03);
    usleep(5000);

    // Send 8 bits command
    Write4bit(SpiI, 0x03);
    usleep(150);

    // Change to 4 bits mode
    Write4bit(SpiI, 0x02);

    return Status;
}

int Write4bit(XSpiPs *SpiI, uint8_t data)
{
    int Status;
    uint8_t data_temp;

    // Set RS
    Status = MCP23S17_Write(SpiI, GPIOB, 0x00);
    if(Status != XST_SUCCESS){
        printf("Error: Set RS\n");
    }

    // Set Data 7-4
    data_temp = data << 4;
    //xil_printf("Set data 7-4: %x\n\r", data_temp);

    Status = MCP23S17_Write(SpiI, GPIOB, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Set data 7-4\n");
    }

    // Set E
    Status = MCP23S17_Write(SpiI, GPIOB, data_temp | 0x08);
    if(Status != XST_SUCCESS){
        printf("Error: Set E\n");
    }



    usleep(1);

    // Clear E
    //data_temp = data_temp & 0xF7;
    Status = MCP23S17_Write(SpiI, GPIOB, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Clear E\n");
    }

    return Status;


}


int LcdCommandWrite(XSpiPs *SpiI, uint8_t data)
{
    int Status;
    uint8_t data_temp;

    // Set RS
    Status = MCP23S17_Write(SpiI, GPIOB, 0x00);
    if(Status != XST_SUCCESS){
        printf("Error: Set RS\n");
    }

    // Set Data 7-4
    data_temp = data & 0xF0;
    //xil_printf("Set data 7-4: %x\n\r", data_temp);

    Status = MCP23S17_Write(SpiI, GPIOB, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Set data 7-4\n");
    }

    // Set E
    Status = MCP23S17_Write(SpiI, GPIOB, data_temp | 0x08);
    if(Status != XST_SUCCESS){
        printf("Error: Set E\n");
    }

    usleep(1);

    // Clear E

    Status = MCP23S17_Write(SpiI, GPIOB, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Clear E\n");
    }

    // Set Data 3-0
    data_temp = data << 4;
    //xil_printf("Set Data 3-0: %x\n\r", data_temp);

    Status = MCP23S17_Write(SpiI, GPIOB, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Set data 3-0\n");
    }

    // Set E
    Status = MCP23S17_Write(SpiI, GPIOB, data_temp | 0x08);
    if(Status != XST_SUCCESS){
        printf("Error: Clear E\n");
    }

   usleep(1);

    // Clear E
    Status = MCP23S17_Write(SpiI, GPIOB, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Clear E\n");
    }

    return Status;
}


int LcdDataWrite(XSpiPs *SpiI, uint8_t data)
{
    int Status;
    uint8_t data_temp;

    // Set RS
    Status = MCP23S17_Write(SpiI, GPIOB, 0x04);
    if(Status != XST_SUCCESS){
        printf("Error: Set RS\n");
    }

    // Set Data 7-4
    data_temp = data & 0xF0;

    Status = MCP23S17_Write(SpiI, GPIOB, 0x04 |data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Set data 7-4\n");
    }

    // Set E
    Status = MCP23S17_Write(SpiI, GPIOB, 0x0C | data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Set E\n");
    }

    usleep(1);

    // Clear E
    Status = MCP23S17_Write(SpiI, GPIOB, 0x04 | data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Clear E\n");
    }

    // Set Data 3-0
    data_temp = data << 4;
    //xil_printf("Set Data 3-0: %x\n\r", data_temp);

    Status = MCP23S17_Write(SpiI, GPIOB, 0x04 |data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Set data 3-0\n");
    }

    // Set E
    Status = MCP23S17_Write(SpiI, GPIOB, data_temp | 0x0C);
    if(Status != XST_SUCCESS){
        printf("Error: Clear E\n");
    }
   usleep(1);

    // Clear E
    Status = MCP23S17_Write(SpiI, GPIOB, 0x04 | data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: Clear E\n");
    }

    return Status;
}

int DisplayOnOff(XSpiPs *SpiI, uint8_t disp, uint8_t cursor, uint8_t blink)
{
    int Status;

    uint8_t data_temp;

    data_temp = 0x08 | (disp << 2) | (cursor << 1) | blink;

    Status = LcdCommandWrite(SpiI, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: DisplayOnOff\n");
    }    

    return Status;
}

int SetDdramAddress(XSpiPs *SpiI, uint8_t address)
{
    int Status;
    uint8_t data_temp;

    if(address > 0x4F)
        data_temp = 0x4F;
    else
        data_temp = address;

    Status = LcdCommandWrite(SpiI, data_temp | 0x80);
    if(Status != XST_SUCCESS){
        printf("Error: SetDdramAddress\n");
    }    
    return Status;
}

int FunctionSet(XSpiPs *SpiI, uint8_t dl, uint8_t n, uint8_t f)
{
    int Status;

    uint8_t data_temp;

    data_temp = 0x20 | (dl << 4) | (n << 3) | (f << 2);

    Status = LcdCommandWrite(SpiI, data_temp);
    if(Status != XST_SUCCESS){
        printf("Error: FunctionSet\n");
    }    

    return Status;
}

int EntryMode(XSpiPs *SpiI, uint8_t id, uint8_t s)
{
    int Status;

    Status = LcdCommandWrite(SpiI, 0x04 | (id<<1) | s);
    if(Status != XST_SUCCESS){
        printf("Error: EnteryMode\n");
    }    

    return Status;
}

int ClearDisplay(XSpiPs *SpiI)
{
    int Status;

    Status = LcdCommandWrite(SpiI, 0x01);
    if(Status != XST_SUCCESS){
        printf("Error: ClearDisplay\n");
    }    

    return Status;
}

int ReturnHome(XSpiPs *SpiI)
{
    int Status;

    Status = LcdCommandWrite(SpiI, 0x02);
    if(Status != XST_SUCCESS){
        printf("Error: ReturnHome\n");
    }    

    return Status;
}

int MCP4161SetContrast(XSpiPs *SpiI, uint8_t contrast)
{
    int Status;

    uint8_t buff_mcp4146[2];
    buff_mcp4146[0] = 0x00;
    buff_mcp4146[1] = contrast;

    XSpiPs_SetSlaveSelect(SpiI, 0x2);

    Status = XSpiPs_PolledTransfer(SpiI, buff_mcp4146, NULL, sizeof(buff_mcp4146));
	if (Status != XST_SUCCESS) {
		printf("Error: MCP4161_setup()\n");
	} 

    return Status;
}

int LcdCharWrite(XSpiPs *SpiI, char char_data[])
{
    int Status;
    uint8_t data;
    int length = strlen(char_data);

    for(int i=0; i < length; i++)
    {
        data = (uint8_t) char_data[i];

        // Set RS
        Status = MCP23S17_Write(SpiI, GPIOB, 0x04);
        if(Status != XST_SUCCESS){
            printf("Error: Set RS\n");
        }

        // Set Data 7-4

        Status = MCP23S17_Write(SpiI, GPIOB, 0x04 |(data & 0xF0));
        if(Status != XST_SUCCESS){
            printf("Error: Set data 7-4\n");
        }

        // Set E
        Status = MCP23S17_Write(SpiI, GPIOB, 0x0C | (data & 0xF0));
        if(Status != XST_SUCCESS){
            printf("Error: Set E\n");
        }

        usleep(1);

        // Clear E
        Status = MCP23S17_Write(SpiI, GPIOB, 0x04 | (data & 0xF0));
        if(Status != XST_SUCCESS){
            printf("Error: Clear E\n");
        }

        // Set Data 3-0

        Status = MCP23S17_Write(SpiI, GPIOB, 0x04 |(data << 4));
        if(Status != XST_SUCCESS){
            printf("Error: Set data 3-0\n");
        }

        // Set E
        Status = MCP23S17_Write(SpiI, GPIOB, (data << 4) | 0x0C);
        if(Status != XST_SUCCESS){
            printf("Error: Clear E\n");
        }
        usleep(1);

        // Clear E
        Status = MCP23S17_Write(SpiI, GPIOB, 0x04 | (data << 4));
        if(Status != XST_SUCCESS){
            printf("Error: Clear E\n");
        }
    }
    return Status;
}
