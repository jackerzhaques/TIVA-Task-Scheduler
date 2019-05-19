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
#include "driverlib/fpu.h"
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>

//System clock running at 120MHz if 129 or 80MHz if 123
#ifdef PART_TM4C129
    #define SYS_CLK     120000000
#elif PART_TM4C123
    #define SYS_CLK     80000000
#endif

void EnableClock(void);
void EnablePeripherals();
void InitConsole(void);

//Copies val to the buffer as an ascii string. Arg1 is the number of places before decimal, arg2 is after.
void sprintfloat(char *Buffer, float val, int arg1){
    int LeftSide = (int)val;
    int RightSide = (int)(pow(10,arg1) * (val - LeftSide)); //Remove the portion to the left of the decimal
    sprintf(Buffer, "%i.%i", LeftSide, RightSide);
}

void print(){
    UARTprintf("Task 1");
}

void print2(){
    UARTprintf("Task 2");
}

void print3(){
    UARTprintf("Task 3");
}

Task tasks[3];

int main(void)
{
    FPULazyStackingEnable();
    EnableClock();
    EnablePeripherals();

    InitializeTaskScheduler(TIMER0_BASE, SYSCTL_PERIPH_TIMER0, SYS_CLK, INT_TIMER0A);

    tasks[0].period = 3;
    tasks[0].enabled = 1;
    tasks[0].pCallback = print;

    tasks[1].period = 1.5;
    tasks[1].enabled = 1;
    tasks[1].pCallback = print2;

    tasks[2].period = 1;
    tasks[2].enabled = 1;
    tasks[2].pCallback = print3;

    AddTask(&tasks[0]);
    AddTask(&tasks[1]);
    AddTask(&tasks[2]);

    while(1){
    }
}

void EnableClock(void){
#ifdef PART_TM4C129
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                      SYSCTL_OSC_MAIN |
                      SYSCTL_USE_PLL |
                      SYSCTL_CFG_VCO_480), SYS_CLK);
#elif PART_TM4C123
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    uint32_t g_ui32SysClock = SysCtlClockGet ();
#endif
}

/*
    Enables all peripherals needed for this motor driver test
*/
void EnablePeripherals(void){
    InitConsole();
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
