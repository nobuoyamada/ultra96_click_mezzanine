#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include "xgpiops.h"

#include "define.h"
#include "gpio.h"

int GPIO_Init(XGpioPs *Gpio)
{
	XGpioPs_Config *ConfigPtr;
	int Status;

	/* Initialize the Gpio driver. */
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	XGpioPs_CfgInitialize(Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	/* Run a self-test on the GPIO device. */
	Status = XGpioPs_SelfTest(Gpio);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    printf("GPIO configuration done.\n");

    return 1;
}
