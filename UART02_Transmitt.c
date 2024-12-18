
#include "UART02_Transmitt.h"
#include "SysTick_Delay.h"

//We are going to be using the UART2 which is an alternate function of the PD7Tx and PD6Rx pin
void UART2_Init(void)
{
	SYSCTL -> RCGCUART |= 0x04;//Enable the clock for UART02;
	
	SYSCTL -> RCGCGPIO |= 0x08;//We are enabling the Port D for use
	
	UART2 -> CTL &= ~0x01;//We are going to be disabling the module before configuring
	
	//UART2 -> CTL &= ~0x0020;//We are going to be using the system clock (50MHz). The last 16 bits are reserved
	
	UART2 -> IBRD = 104;//We are going to be using 9600 for our baud rate(Integer)
	
	UART2 ->FBRD = 11; //We are going to be using 9600 for our baud rate(fraction)
	
	//Page 916 for the data length, parity and such
	UART2 -> LCRH = 0x60; //Basically we want a lenth of 8 bits with no parity
	
	UART2 -> CTL = 0x0301;//We are enabling the Transmitter and reciever
	
	//GPIO Pins
	GPIOD->LOCK = 0x4C4F434B;  // Unlock the GPIOLOCK register with the magic value
	GPIOD->CR |= 0xC0;         // Allow changes to PD6 (0x40) and PD7 (0x80)
	GPIOD->LOCK = 0;           // Relock the GPIOLOCK register for safety

	GPIOD -> AFSEL |= 0x0C;//Alternate function for pin 6 and 7;
	GPIOD -> DEN |= 0x0C;//Digitally enable pin 6 and 7
	GPIOD -> PCTL &= ~(0xFF000000);
	GPIOD -> PCTL |= 0x11000000;//For PB6 and PB7, the Uart communication is the first alternate function for each.
	
}
void UART2_Transmitt(char value)
{
	while((UART2->FR & (1 << 5)) != 0);
	UART2->DR = value;
}
char UART2_Recieve(void)
{
	while((UART2 -> FR & 0x10) != 0);
	return (char)(UART2 -> DR & 0xFF);
	
}

void print_data(char *str)
{
	//while(UART2 ->FR & (1 << 5) != 0);
	//UART2 ->ICR = 0x7FF;
	while(*str)
	{
		UART2_Transmitt(*(str++));
		SysTick_Delay1us(20);
	}
	
}

void Delay(unsigned long counter)
{
	unsigned long i = 0;
	for(i = 0; i<counter*1000; i++);
}
