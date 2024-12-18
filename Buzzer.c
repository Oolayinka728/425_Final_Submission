//We are using PF2 for the buzzer GPIO pin
#include "Buzzer.h"
#include "TM4C123GH6PM.h"
const uint8_t BUZZER_OFF = 0x00;
const uint8_t BUZZER_ON = 0x04;


const double C4_NOTE = 261.6;
const double D4_NOTE = 293.7;
const double E4_NOTE = 329.6;
const double F4_NOTE = 349.2;
const double G4_NOTE = 392.0;
const double A4_NOTE = 440.0;
const double B4_NOTE = 493.0;
const double A5_NOTE = 960.0;
const double G5_NOTE = 853.0;

void Buzzer_Init(void)
{
	//Enable the clock to Port F
	SYSCTL->RCGCGPIO |= 0x20;
	
	//Set PF2 as an output GPIO pin
	GPIOF->DIR |= 0x04;
	
	//Configure PC4 to function as a GPIO pin
	GPIOF->AFSEL &= ~0x04;
	
	//Enable digital functionality for PF2
	GPIOF->DEN |= 0x04;
}



void Buzzer_Output (uint8_t buzzer_value)
{
	GPIOF -> DATA = (GPIOF->DATA & ~0x04) | buzzer_value;
}

void Play_Note(double note, unsigned int duration)
{
	//Calculate the period of the note in microseconds
	int period_us = (int)(((double)1/note) * ((double)1000000));
	
	//Calculate the half period of the note in mircrseconds
	int half_period_us = period_us / 2;
	
	//Generate a square wave for the specific duration
	for(unsigned int i = 0; i < duration; i++)
	{
		Buzzer_Output(BUZZER_ON);
		SysTick_Delay1us(half_period_us);
		Buzzer_Output(BUZZER_OFF);
		SysTick_Delay1us(half_period_us);
	}
}

void Note_Controller(uint8_t push_button)//Modify this later on for the sensor.`
{
	if ( push_button == 0x00)
	{
		//Do nothing
	}
	else if(push_button == 0x10)
	{

	}else{
		//Do nothing
	}
	
}
void alarm(void)
{
	static uint8_t A5_state = 1;
	if(A5_state == 1){
		Play_Note(G5_NOTE, 200);
		A5_state = 0;
	}
	else if (A5_state ==0)
	{
		Play_Note(A5_NOTE, 200);
		A5_state = 1;
	}
	
}