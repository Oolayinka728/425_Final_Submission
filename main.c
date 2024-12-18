
#include <stdio.h>
#include "TM4C123GH6PM.h"
#include "system_tm4c123.h" 
#include "GPIO.h"
//#include "Timer_Capture_Interrupt.h"
#include "SysTick_Delay.h"
#include "UART1.h"
#include "Buzzer.h"
#define THRESHOLD 2000  //Distance threshold
char message[30];
uint16_t distance;
/*volatile uint32_t measured_time = 0; // Shared variable for time difference
volatile float measured_distance = 0;
*/
int main(void)
{
    // Initialize peripherals
    LED_Init();
    Push_button_Init();
    SysTick_Delay_Init();
    UART1_Init();
    Buzzer_Init();

	
		//PWM0_0_Update_Duty_Cycle(5000);

    // Send startup message
   UART1_Output_String("System Initialized. Measuring Distance...\n");
    UART1_Output_Newline();

    while (1)
    {
				uint8_t button_status = Push_Button_Status();
				Note_Controller(button_status);
				//SysTick_Delay1ms(50);
			
			
				uint16_t distance = Distance_UART(); // Get distance from US-100 sensor
				if (distance > 0) {
            // Format the distance into a string
            char buffer[30];
            sprintf(buffer, "Distance: %u cm\n", distance);  // Format distance

            // Send the distance as a string via UART
            UART1_Output_String(buffer);  // Send the formatted string over UART
   					UART1_Output_Newline();  // Send a newline after the data
					if(button_status ==0x10)
					{
						if (distance >= THRESHOLD){
								Play_Note(BUZZER_ON, 50);
								
						}else{
							Play_Note(BUZZER_OFF,50);
						}
					}
					else if(button_status == 0x20)
					{
						Play_Note(BUZZER_OFF,50);
					}
						
					
					
        } else {
            // If no valid response or checksum error, send an error message
            UART1_Output_String("Error: No valid response from sensor.\n");
            UART1_Output_Newline();
					  SysTick_Delay1ms(200);  // Wait .5 second before the next measurement

        }
        
    }

}




