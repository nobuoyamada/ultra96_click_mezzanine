#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include "xiicps.h"

#include "define.h"
#include "i2c.h"

int Init(XIicPs *Iic)
{
    int Status;
    XIicPs_Config *Config;  /**< configuration information for the device */

    Config = XIicPs_LookupConfig(IIC_DEVICE_ID);
    if(Config == NULL){
        printf("Error: XIicPs_LookupConfig()\n");
        return XST_FAILURE;
    }

    Status = XIicPs_CfgInitialize(Iic, Config, Config->BaseAddress);
    if(Status != XST_SUCCESS){
        printf("Error: XIicPs_CfgInitialize()\n");
        return XST_FAILURE;
    }

    Status = XIicPs_SelfTest(Iic);
    if(Status != XST_SUCCESS){
        printf("Error: XIicPs_SelfTest()\n");
        return XST_FAILURE;
    }

    XIicPs_SetSClk(Iic, IIC_SCLK_RATE);
    printf("I2C configuration done.\n");

    return XST_SUCCESS;
}



int i2c_write(XIicPs *Iic, u8 command, u16 i2c_adder)
{
    int Status;
    u8 buffer[4];
    buffer[0] = command;

    Status = XIicPs_MasterSendPolled(Iic, buffer, 1, i2c_adder);

    if(Status != XST_SUCCESS){
    	print("I2C write error!\n\r");
        return XST_FAILURE;
    }

    // Wait until bus is idle to start another transfer.
    while(XIicPs_BusIsBusy(Iic)){
        /* NOP */
    }
    return XST_SUCCESS;
}

int i2c_read(XIicPs *Iic, u8* buff, u32 len, u16 i2c_adder)
{
    int Status;

    Status = XIicPs_MasterRecvPolled(Iic, buff, len, i2c_adder);

    return Status;
}

int i2c_switch_on(XIicPs *Iic, u8 channel)
{
    int Status;

	switch(channel){
		case 0: Status = i2c_write(Iic, 0x01, TCA9548A_ADDRESS); break;
		case 1: Status = i2c_write(Iic, 0x02, TCA9548A_ADDRESS); break;
		case 2: Status = i2c_write(Iic, 0x04, TCA9548A_ADDRESS); break;
		case 3: Status = i2c_write(Iic, 0x08, TCA9548A_ADDRESS); break;
		case 4: Status = i2c_write(Iic, 0x10, TCA9548A_ADDRESS); break;
		case 5: Status = i2c_write(Iic, 0x20, TCA9548A_ADDRESS); break;
		case 6: Status = i2c_write(Iic, 0x40, TCA9548A_ADDRESS); break;
		case 7: Status = i2c_write(Iic, 0x80, TCA9548A_ADDRESS); break;
		default: Status = i2c_write(Iic, 0x00, TCA9548A_ADDRESS); break;
	}
    if(Status != XST_SUCCESS){
    	print("i2c_switch_on error!\n\r");
    }
    return Status;
}



