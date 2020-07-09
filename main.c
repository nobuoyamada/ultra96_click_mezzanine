/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include "xiicps.h"
#include "xspips.h"
#include "xgpiops.h"

#include "acc_sensor.h"
#include "gyro_sensor.h"
#include "i2c.h"
#include "define.h"
#include "spi.h"
#include "gpio.h"
#include "lcd_click_mini.h"

XIicPs Iic;
XSpiPs SpiI;
XGpioPs Gpio;

typedef union{
  s16 i16bit[3];
  u8 u8bit[6];
} axis3bit16_t;

static axis3bit16_t data_raw_acc;
static axis3bit16_t data_raw_gyro;
static float acceleration_mg[3];
static float angular_rate_mdps[3];

int main()
{
    char acc_data[10];

    // Init each parameter
    init_platform();
    Init(&Iic);
    SPI_Init(&SpiI);
    memset(data_raw_acc.u8bit, 0x00, 3*sizeof(s16));
    memset(data_raw_gyro.u8bit, 0x00, 3*sizeof(s16));

    // Init acc/gyro sensor
    i2c_switch_on(&Iic, CH0);
    acc_on_off(&Iic, ACC_ON);
    gyro_on_off(&Iic, GYRO_ON);

    //lsm6dsl_regdump(&Iic);

    // Set contrast of character LCD
    MCP4161SetContrast(&SpiI, 0x40);

    // character LCD initialize
    LcdPinInit(&SpiI);
    usleep(5000);
    FunctionSet(&SpiI, DL_4BIT, N_2LINE, F_5x8);
    usleep(1000);
    DisplayOnOff(&SpiI, DISPON, CURSOR_OFF, BLINK_OFF);
    usleep(500);
    ClearDisplay(&SpiI);
    usleep(2000);
    EntryMode(&SpiI, DIR_LEFT, NO_SCR_SHIFT);
    usleep(5000);

    //LcdCharWrite(&SpiI, "ABCDEFGHIJ");
    
    while(1){
        lsm6dsl_acc_raw_get(&Iic, data_raw_acc.u8bit);
        //lsm6dsl_gyro_raw_get(&Iic, data_raw_gyro.u8bit);

        acceleration_mg[0] = lsm6dsl_from_fs2g_to_mg(data_raw_acc.i16bit[0]);
        acceleration_mg[1] = lsm6dsl_from_fs2g_to_mg(data_raw_acc.i16bit[1]);
        acceleration_mg[2] = lsm6dsl_from_fs2g_to_mg(data_raw_acc.i16bit[2]);
        //angular_rate_mdps[0] = lsm6dsl_from_fs2000dps_to_mdps(data_raw_gyro.i16bit[0]); 
        //angular_rate_mdps[1] = lsm6dsl_from_fs2000dps_to_mdps(data_raw_gyro.i16bit[1]); 
        //angular_rate_mdps[2] = lsm6dsl_from_fs2000dps_to_mdps(data_raw_gyro.i16bit[2]); 

        ReturnHome(&SpiI);
        LcdCharWrite(&SpiI, "X:");
        sprintf(acc_data, "%-6.1f", acceleration_mg[0]);
        LcdCharWrite(&SpiI, acc_data);

        LcdCharWrite(&SpiI, "Y:");
        sprintf(acc_data, "%-6.1f", acceleration_mg[1]);
        LcdCharWrite(&SpiI, acc_data);

        SetDdramAddress(&SpiI, 0x40);

        LcdCharWrite(&SpiI, "Z:");
        sprintf(acc_data, "%-7.1f", acceleration_mg[2]);
        LcdCharWrite(&SpiI, acc_data);

        sleep(1);

        /*
        
        for(int i=0; i<6; i=i+2)
            printf("data_raw_acc.i16bit[%d]: %02x%02x\n\r", i, data_raw_acc.u8bit[i+1],data_raw_acc.u8bit[i]);
        
        for(int i=0; i<3; i++)
            printf("acceleration_mg[%d]: %4.2f\n\r", i, acceleration_mg[i]);

        for(int i=0; i<6; i=i+2)
            printf("data_raw_gyro.i16bit[%d]: %02x%02x\n\r", i, data_raw_gyro.u8bit[i+1],data_raw_gyro.u8bit[i]);   

        for(int i=0; i<3; i++)
            printf("angular_rate_mdps[%d]: %4.2f\n\r", i, angular_rate_mdps[i]);
        
    */
    }

    cleanup_platform();
    return 0;
}
