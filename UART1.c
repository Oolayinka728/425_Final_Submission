#include "Timer_Capture_Interrupt.h"
#include "GPIO.h"
#include "SysTick_Delay.h"
#include "UART1.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
void UART1_Init(void)
{
    // Enable the clock to the UART1 module by setting the R1 bit (Bit 1) in the RCGCUART register
    SYSCTL->RCGCUART |= 0x02;
    
    // Enable the clock to Port B by setting the R1 bit (Bit 1) in the RCGCGPIO register
    SYSCTL->RCGCGPIO |= 0x02;
    
    // Disable the UART1 module before configuration by clearing the UARTEN bit (Bit 0) in the CTL register
    UART1->CTL &= ~0x0001;
    
    // Configure the UART1 module to use the system clock (50MHz) divided by 16 by clearing the HSE bit (Bit 5) in the CTL register
    UART1->CTL &= ~0x0020;
    
    // Set the baud rate by writing to the DIVINT (integer) and DIVFRAC (fractional) fields
    // Assuming the same calculation as in UART1
    UART1->IBRD = 325;  // Integer part of the baud rate divisor(9600)
    UART1->FBRD = 33;   // Fractional part of the baud rate divisor
    
    // Configure the line control for 8-bit data, no parity, and one stop bit
    UART1->LCRH |= 0x60;  // Set WLEN = 3 (8-bit data length)
    UART1->LCRH &= ~0x10; // Clear PEN (Parity Enable) for no parity
    UART1->LCRH &= ~0x08; // Clear STP2 (1 stop bit)
    UART1->LCRH &= ~0x02; // Clear EPS (Even Parity Select)
    
    // Enable UART1 by setting the UARTEN bit (Bit 0) in the CTL register
    UART1->CTL |= 0x01;
		
    
    // Configure GPIO Port B pins for UART1 (PB0 = U1Rx, PB1 = U1Tx)
    GPIOB->AFSEL |= 0x03;       // Enable alternate function on PB0 and PB1
    GPIOB->PCTL &= ~0x000000FF; // Clear PCTL for PB0 and PB1
    GPIOB->PCTL |= 0x00000011;  // Set PCTL to select UART1 for PB0 and PB1
    GPIOB->DEN |= 0x03;         // Enable digital functionality on PB0 and PB1
}
void UART1_Output_Character(char data)
{
	while ((UART1 -> FR & UART1_TRANSMIT_FIFO_FULL_BIT_MASK) != 0);
	
	UART1 -> DR = data;
}
char UART1_Input_Character(void)
{	
	while((UART1->FR & UART1_RECEIVE_FIFO_EMPTY_BIT_MASK) != 0);
	return (char)(UART1->DR & 0xFF);
}

void UART1_Output_String(char *pt)
{
	while(*pt)
	{
		UART1_Output_Character(*pt);
		pt++;
	}
}
void UART1_Input_String(char *buffer_pointer, uint16_t buffer_size) 
{
	int length = 0;
	char character = UART1_Input_Character();
	
	while(character != UART1_CR)
	{
		if (character == UART1_BS)
		{
			if (length)
			{
				buffer_pointer--;
				length--;
				UART1_Output_Character(UART1_BS);
			}
		}
		else if (length < buffer_size)
		{
			*buffer_pointer = character;
			buffer_pointer++;
			length++;
			UART1_Output_Character(character);
		}
		character = UART1_Input_Character();
			
	}
		*buffer_pointer = 0;
}

void UART1_Output_Newline(void)
{
	UART1_Output_Character(UART1_CR);
	UART1_Output_Character(UART1_LF);
}

uint16_t Distance_UART(void) {
    uint8_t rx_buffer[2];  // Buffer to hold the 2-byte distance data

    // Send the command to initiate measurement (0x55)
    UART1_Output_Character(0x55);  

    // Wait for the sensor to respond (you may need to adjust this delay)
    SysTick_Delay1ms(700);  // Wait 500ms for the sensor to process (adjust as needed)

    // Read 2 bytes from the UART (the 16-bit distance value)
    for (int i = 0; i < 2; i++) {
        rx_buffer[i] = UART1_Input_Character();
    }

    // Combine the two bytes into a 16-bit distance (assuming little-endian format)
    uint16_t distance = (rx_buffer[1] << 8) | rx_buffer[0];

    // Debugging: Print the received raw data
    UART1_Output_String("Received raw data: ");
    for (int i = 0; i < 2; i++) {
        char byte_str[5];
        sprintf(byte_str, "%02X ", rx_buffer[i]);  // Print each byte as hex
        UART1_Output_String(byte_str);
    }
    UART1_Output_Newline();

    return distance;  // Return the distance value
}





