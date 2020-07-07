#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include "xiicps.h"

#include "acc_sensor.h"
#include "gyro_sensor.h"
#include "define.h"
#include "i2c.h"

int gyro_on_off(XIicPs *Iic, u8 on_off){

	int Status;
	u8 data;
    data = 0xFF;
    Status = lsm6dsl_read_reg(Iic, &data, CTRL2_G, 1);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    switch(on_off){
        case GYRO_ON: 	data |= 0x40; break;
        case GYRO_OFF:   data &= ~0x40; break;
        default:        data &= ~0x40; break;
    }
    printf("on_off: %x\n\r", on_off);
    printf("data: %x\n\r", data);
	//Status = XIicPs_MasterSendPolled(Iic, buffer, 2, LSM6DSL_ADDRESS);
    Status = lsm6dsl_write_reg(Iic, &data, CTRL2_G, 2);

    if(Status != XST_SUCCESS){
    	print("acc_on error!\n\r");
    }
    return Status;
}

int lsm6dsl_gyro_raw_get(XIicPs *Iic, u8* buff){

	int Status;
	u8	address[1];

	address[0] = OUTX_L_G;

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

float lsm6dsl_from_fs2000dps_to_mdps(s16 lsb)
{
  return ((float)lsb * 70.0f);
}
