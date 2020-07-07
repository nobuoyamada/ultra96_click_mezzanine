#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include "xiicps.h"

#include "acc_sensor.h"
#include "define.h"
#include "i2c.h"

int lsm6dsl_read_reg(XIicPs *Iic, u8* buff, u8 address, u32 len)
{
    int Status;

	XIicPs_SetOptions(Iic, XIICPS_REP_START_OPTION);

	// Send the Data.
	Status = XIicPs_MasterSendPolled(Iic, &address, 1, LSM6DSL_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Wait until bus is idle to start another transfer.
	if (!(Iic->IsRepeatedStart)) {
		while (XIicPs_BusIsBusy(Iic));
	}

	// Receive the Data.
	Status = XIicPs_MasterRecvPolled(Iic, buff, len, LSM6DSL_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
    Iic->IsRepeatedStart = 0;
    //printf("Iic->IsRepeatedStart: %d\n",Iic->IsRepeatedStart);
	return XST_SUCCESS;
}

int lsm6dsl_write_reg(XIicPs *Iic, u8* data, u8 address, u32 len)
{
    int Status;

    u8 *buffer;
    *buffer = address;
    *(buffer+1) = *data;

    Status = XIicPs_MasterSendPolled(Iic, buffer, len, LSM6DSL_ADDRESS);

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

int acc_on_off(XIicPs *Iic, u8 on_off){

	int Status;
	u8 data;
    data = 0xFF;
    //printf("on_off: %x\n\r", on_off);
    //printf("data: %x\n\r", data);
    Status = lsm6dsl_read_reg(Iic, &data, CTRL1_XL, 1);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
    printf("data: %x\n\r", data);
    switch(on_off){
        case ACC_ON: 	data |= 0x40; break;
        case ACC_OFF:   data &= ~0x40; break;
        default:        data &= ~0x40; break;
    }
    printf("data: %x\n\r", data);
	//Status = XIicPs_MasterSendPolled(Iic, buffer, 2, LSM6DSL_ADDRESS);
    Status = lsm6dsl_write_reg(Iic, &data, CTRL1_XL, 2);

    if(Status != XST_SUCCESS){
    	print("acc_on error!\n\r");
    }
    return Status;
}

void lsm6dsl_regdump(XIicPs *Iic)
{
	u8	buff[128];

    i2c_write(Iic, 0x00, LSM6DSL_ADDRESS);
    i2c_read(Iic, buff, 128, LSM6DSL_ADDRESS);

    for(int i=0; i<8; i++){
    	for(int j=0; j<16; j++){
    		printf("%02x ", buff[i*16+j]);
    	}
    	printf("\n\r");
    }
}

int lsm6dsl_acc_raw_get(XIicPs *Iic, u8* buff){

	int Status;
	u8	address[1];

	address[0] = OUTX_L_XL;

	XIicPs_SetOptions(Iic, XIICPS_REP_START_OPTION);

	/*
	 * Send the Data.
	 */
	Status = XIicPs_MasterSendPolled(Iic, address, 1, LSM6DSL_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait until bus is idle to start another transfer.
	 */
	if (!(Iic->IsRepeatedStart)) {
		while (XIicPs_BusIsBusy(Iic));
	}

	/*
	 * Receive the Data.
	 */
	Status = XIicPs_MasterRecvPolled(Iic, buff,
					  6, LSM6DSL_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Wait until bus is idle to start another transfer.
	 */
	while (XIicPs_BusIsBusy(Iic));

	return XST_SUCCESS;
}

float lsm6dsl_from_fs2g_to_mg(s16 lsb)
{
  return ((float)lsb * 0.061f);
}
