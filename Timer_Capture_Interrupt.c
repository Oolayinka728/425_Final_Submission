#include "TM4C123GH6PM.h" // Include the header file for the TM4C123GXL
#include "UART1.h"
#include "Timer_Capture_Interrupt.h"
#include "GPIO.h"
#include "SysTick_Delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
/*
//char message[60];
//We are using 1k, 220 and 470(1.7K)(top 3) and (3.3k)(bottom) for the echo pin
volatile uint32_t rising_edge = 0;
volatile uint32_t falling_edge = 0;
volatile uint8_t edge_state = 0;

void GPIO_Sensor_Init(void)
{
  SYSCTL->RCGCGPIO |= 0x02;  	// Enable Port B clock
	SYSCTL->RCGCGPIO |= 0x10;//Enable the clock for Port E
	
	           
  GPIOB->AFSEL |= 0x04;          // Enable alternate function for PB2
  GPIOB->PCTL &= ~0x00000F00;    // Clear PCTL for PB2
  GPIOB->PCTL |= 0x00000700;     // Set PB2 to T3CCP0
  GPIOB->DEN |= 0x04; 
	GPIOB->DIR &= ~0x04;// Set PB2 as input
	// Enable digital function for PB2
	//GPIOB -> DATA &= ~0x04;
   
	//Configure PE0 as output as Outputs
	
	GPIOE -> AFSEL &= ~0x01;//Set PE0 to output
	GPIOE -> DIR |= 0x01;// Set the direction of PE0 as output (Trigger)
	GPIOE -> DEN |= 0x01;//digitally enable the PE0 
	GPIOE -> DATA &= ~0x01;
}
void debug_LED(void)
{
	SYSCTL->RCGCGPIO |= 0x10;
	GPIOE -> AFSEL &= ~0x10;
	GPIOE -> DIR |=0x10;
	GPIOE -> DEN |=0x10;
	GPIOE -> DATA ^= 0x10;
	
}
void Timer3_Init(void) {
    SYSCTL->RCGCTIMER |= 0x08; // Enable Timer3 clock
    TIMER3->CTL &= ~0x01;      // Disable Timer3A during configuration
    TIMER3->CFG = 0x00;        // Configure as 32-bit timer
    TIMER3->TAMR = 0x17;       // Capture mode/edge time mode(Mind you, we are counting up, not down)
    TIMER3->CTL &= ~0x0C;      // Clear capture edge settings
    TIMER3->CTL |= 0x0C;       // Capture on both edges (set to rising first)
		TIMER3->ICR |= 0x04;//This clears the interrupt flag set it is set
    TIMER3->IMR |= 0x04; 	// Enable capture event interrupt
		 // Enable Timer3A interrupt in NVIC
		NVIC->IPR[8] = (NVIC ->IPR[8] & 0x00FFFFFF) | (1 << 29);      // Set priority to 2
		NVIC->ISER[1] |= (1 << 3); // IRQ 35 corresponds to Timer3A
    TIMER3->CTL |= 0x01;       // Enable Timer3 
}

void Trigger_Pulse(void)// This trigger pulse is configured to be 10 us because that is the needed pulse to activate the sensor
{
	
	GPIOE -> DATA |= 0x01;
	SysTick_Delay1us(10);
	GPIOE -> DATA &= ~0x01;
}
void Timer3A_Handler(void) {
    //static uint32_t last_edge = 0; // Stores the previous edge
		static uint8_t pulse_sent = 0;
    uint32_t current_edge = 0;    // Stores the current edge
    uint32_t time_difference = 0;
		if(!pulse_sent)
		{
			Trigger_Pulse();
			pulse_sent = 1;
			debug_LED();
		}
		
		//debug_LED();
		//SysTick_Delay1ms(500);

    if (TIMER3->MIS & 0x04) { // Check if a capture event occurred
        current_edge = TIMER3->TAR; // Read the captured timer value
        GPIOE->DATA ^= 0x10;        // Toggle debug LED (PE4) to indicate ISR execution

        if (edge_state == 0) {      // Rising edge detected
            rising_edge = current_edge;
            edge_state = 1;         // Next edge is falling
            TIMER3->CTL &= ~0x04;   // Switch to capture falling edge
        } else {                    // Falling edge detected
            falling_edge = current_edge;
            edge_state = 0;         // Next edge is rising
            TIMER3->CTL |= 0x04;    // Switch to capture rising edge

            // Calculate time difference (account for timer overflow)
            if (falling_edge >= rising_edge) {
                time_difference = falling_edge - rising_edge;
            } else {
                time_difference = (0xFFFFFFFF - rising_edge) + falling_edge + 1;
            }

            // Use or store the time_difference (e.g., calculate distance)
            float distance = Calculate_Distance(time_difference);
						
						char buffer[30];
						sprintf(buffer, "%.2f", distance);
							UART1_Output_String(buffer);	
							UART1_Output_Newline();
							pulse_sent =0;
        }
				SysTick_Delay1ms(60);
        TIMER3->ICR = 0x01; // Clear the capture event interrupt flag
    }
}
float Calculate_Distance(uint32_t count)
{
	float time_seconds = count * (1/(50e06));
	float distance = (time_seconds * 343.0)/2;
	return distance;
	
}
*/



