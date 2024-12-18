//PA 2 and A3 are the led outputs(A2 for Green LED, A3 for RED LED)
//560 ohm resistor values for the LEDS.
//PA4 and PA5 are the button inputs(A5 for LED Green, A4 for LED RED)

//PB2 is used for the trigger button on the sensor
//PB3 is used for the echo button on the sensor

#include <stdint.h>
#include <stdio.h>
#include "GPIO.h"
#include "SysTick_Delay.h" 
#include "UART1.h"
#include "Buzzer.h"

#define THRESHOLD 2000  //Distance threshold


volatile uint8_t led_state = 0x00;

void LED_Init(void)
{
	SYSCTL -> RCGCGPIO |= 0x01;
	GPIOA -> DIR |= 0x0C;
	GPIOA -> AFSEL &= ~0x0C;
	GPIOA -> DEN |= 0x0C;
	GPIOA -> DATA &= ~0x0C;
}
void Push_button_Init(void)
{
	SYSCTL -> RCGCGPIO |= 0x01;
	GPIOA -> DIR &= ~0x30;
	GPIOA -> AFSEL &= ~0x30;
	GPIOA -> DEN |= 0x30;
	GPIOA -> PUR |= 0x30;
}

void LED_Output (uint8_t led_value)
{
	GPIOA -> DATA = (GPIOA -> DATA & ~0x0C) | (led_value & 0x0C);
}

uint8_t Push_Button_Status(void)
{
	uint8_t status = (GPIOA -> DATA & 0x30);
	SysTick_Delay1ms(5);
	return status;
}

void LED_Controller(uint8_t Pressed_button)
{
	LED_Output(0x00);
	if (Pressed_button == 0x10)
	{
		led_state = 0x04;//Turn red Led on
	
	}
	else if (Pressed_button == 0x20)
	{
		led_state = 0x08;//Turn green led on
	}
	else{
		
	
	}
	
	/*switch(Pressed_button)
	{
		case 0x00:
			break;
		
		case 0x10:
			led_state = 0x04;//Turn on Green LED
			break;
		
		case 0x20:
			led_state = 0x08;//Turn on RED LED
			break;
		
		default:
			break;
		
	}
	*/
	
	LED_Output(led_state);
}
