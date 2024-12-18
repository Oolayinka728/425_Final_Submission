#include "Timer_Sensor.h"
#include "SysTick_Delay.h"
#include "TM4C123GH6PM.h"
#include "UART1.h"

#include "GPIO.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
void int_to_str(uint32_t num, char *str);
#define Speed_of_Sound 34300 //Speed of Sound in cm/short 
char message[30];
volatile uint32_t rising_edge =0;
volatile uint32_t falling_edge =0;
void(*Timer_task)(void);

void Timer3_Sensor_Init(void(*task)(void))
{
	//ISR
	Timer_task = task;
	
	//We are setting PB3 as our input for the capture mode
	SYSCTL -> RCGCGPIO |= 0x02;//Enable the clock for Port B.
	
	GPIOB -> AFSEL |= 0x08;// We are setting a 1 to B3  to access it alternate functionality
	GPIOB -> DIR &= ~ 0x08;// Setting the pin to input(Echo)
	GPIOB -> DEN |= 0x08; //Digitally enable the pin
	GPIOB -> PCTL &= ~0x000F0000; //Go to page 651 of Datasheet to see what the pins for the alternate functions are. We are initialing the pins for PB6 for use
	GPIOB -> PCTL |= 0x00700000;//On the data sheet, it say an hex value of 7 will enable the T0CCP0
	
	//We are going to set PB2 as the output
	GPIOB -> AFSEL &= ~0x04;//Set PB2 to output
	GPIOB -> DIR |= 0x04;// Set the direction of PB2 as output (Trigger)
	GPIOB -> DEN |= 0x04;//digitally enable the PB2 
	
	
	//Page 722.
	//We are going to be using the TIMER3 for the 
	SYSCTL -> RCGCTIMER |= 0x08;
	//Before configuring the timer, disable the timer
	TIMER3 -> CTL &= ~0x01;//We are clearing the Bit 0 of the CTL register
	
	TIMER3 -> CFG |= 0x04;// Select a 16 bit counter from a 16/32 bit counter
	
	TIMER3 -> TAMR = (TIMER3->TAMR & ~0x03) | 0x03;//GPTMTnMR is the TAMR register.The register puts the timer in capture mode. Then bit 2 set to 1 so the it is in input-edge time mode
	
	TIMER3 -> TAMR |=0x04;//We are setting the time in edge-time mode
	
	TIMER3 -> CTL &= ~0x0C;// Capture the event at both the rising and falling edge of the event
	
	TIMER3 -> CTL |= 0x0C; //Clear the capture even interrupt
	
	TIMER3 -> ICR = 0x04;// Clear any pending interrupts
	
	TIMER3 -> IMR |= 0x04;//This enable a capture interrupt
	
	NVIC -> ISER[0] |= (1 << 35);//Enable interrupt for Timer3 (Interruput 23)
	
	
	
	TIMER3 -> CTL |= 0x01;// Turn on the Timer
}

void Trigger_Pulse(void)// This trigger pulse is configured to be 10 us because that is the needed pulse to activate the sensor
{
	GPIOB -> DATA |= 0x04;
	SysTick_Delay1us(10);
	GPIOB -> DATA &= ~0x04;
	
}
void Timer3_Handler(void)
{
	static uint8_t edge_state = 0;//0 means it is rising, and 1 means it is falling
	GPIOF -> DATA ^=0x02;
	if (TIMER3 -> MIS & 0x04)//If the interrupt flag get triggered
	{
		TIMER3->ICR |= 0x04;//Clear the interrupt
		if(edge_state ==0)
		{
			rising_edge = TIMER3 -> TAR;
			edge_state = 1;//means next state is falling
		}
		else{
			
			falling_edge = TIMER3 -> TAR;
			edge_state = 0;//means next stage is rising
		}
		
		
		(*Timer_task)();//Perform the user defined task;
	}
}

void calculate_distance(void)
{
	Trigger_Pulse();
		//Clear the capture mode for TIMER0A
			
		uint32_t time_difference;
		if (falling_edge >= rising_edge)
		{
			time_difference = falling_edge - rising_edge;
			
		}else{
				time_difference = (0xFFFF - rising_edge) + falling_edge + 1;
		}
		uint32_t distance = (time_difference * Speed_of_Sound) /(2 * 1000000);//We are multipling the duratino of the pulse signal by
		//the speed of sound, and we are dividing by 2 * 1000000 becasue the speed of sound is measured in second not mircosecond.
		//sprintf(message, "Distance = %d cm", distance);
		UART1_Output_String("ABC");
		UART1_Output_Newline();
}

