//Project includes
#include "TaskScheduler/TaskScheduler.h"

//Standard includes
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

//Tivaware includes
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

//System clock running at 120MHz
#define SYS_CLK     120000000

void EnableClock(void);
void EnablePeripherals();
void InitConsole(void);

//Copies val to the buffer as an ascii string. Arg1 is the number of places before decimal, arg2 is after.
void sprintfloat(char *Buffer, float val, int arg1){
    int LeftSide = (int)val;
    int RightSide = (int)(pow(10,arg1) * (val - LeftSide)); //Remove the portion to the left of the decimal
    sprintf(Buffer, "%i.%i", LeftSide, RightSide);
}

int main(void)
{
    EnableClock();
    EnablePeripherals();
}

void EnableClock(void){

    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                      SYSCTL_OSC_MAIN |
                      SYSCTL_USE_PLL |
                      SYSCTL_CFG_VCO_480), SYS_CLK);
}

/*
    Enables all peripherals needed for this motor driver test
*/
void EnablePeripherals(void){
    InitConsole();

    InitializeTaskScheduler(TIMER0_BASE, SYS_CLK, INT_TIMER0A);

    while(1){
        UARTprintf("Hello world!");
        SysCtlDelay(SYS_CLK / 3);
    }
}

//Initializes UART0 to be used as a console.
void InitConsole(void){
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}
