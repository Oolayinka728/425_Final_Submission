#include "TM4C123GH6PM.h"
#include "PWM.h"

void PWM0_3_Init(uint16_t period_constant, uint16_t duty_cycle)
{
    // Return from the function if the specific duty_cycle is greater than
    // or equal to the given period. The duty cycle cannot exceed 99%.
    if (duty_cycle >= period_constant) return;

    // Enable the clock to PWM Module 0 by setting the R0 bit (Bit 0) in the RCGCPWM register
    SYSCTL->RCGCPWM |= 0x01;

    // Enable the clock to GPIO Port F by setting the R5 bit (Bit 5) in the RCGCGPIO register
    SYSCTL->RCGCGPIO |= 0x20;

    // Configure the PF3 pin to use the alternate function (M0PWM3)
    GPIOF->AFSEL |= 0x08;       // Enable alternate function on PF3
    GPIOF->PCTL &= ~0x0000F000; // Clear PCTL for PF3
    GPIOF->PCTL |= 0x00005000;  // Set PCTL to select M0PWM3 on PF3

    // Enable the digital functionality for the PF3 pin
    GPIOF->DEN |= 0x08; // Enable digital functionality for PF3

    // Disable the module 0 PWM Generator 1 block (PWM0_1) before
    // configuration by clearing the ENABLE bit (Bit 0) in the PWM0_1_CTL register
    PWM0->_3_CTL &= ~0x01;

    // Configure the counter for the PWM0_1 block to use Count-Down mode
    PWM0->_3_CTL &= ~0x02; // Clear the MODE bit to use down-count mode

    // Set the action for PWM signal on PF3 (M0PWM3)
    // Drive PWM signal high when the counter matches the comparator (CMPA) while counting down
    PWM0->_3_GENA |= 0xC8; // Drive PWM signal low at LOAD and high at CMPA

    // Set the period by writing to the LOAD field (Bits 15 to 0)
    // This determines the number of clock cycles needed to count down to zero
    PWM0->_3_LOAD = (period_constant - 1);

    // Set the duty cycle by writing to the COMPA field (Bits 15 to 0)
    PWM0->_3_CMPA = (duty_cycle - 1);

    // Enable the PWM0_1 block after configuration by setting the Enable bit (Bit 0)
    PWM0->_3_CTL |= 0x01;

    // Enable the PWM0 signal to be passed to PF3 (M0PWM3)
    PWM0->ENABLE |= 0x08; // Enable PWM3
}

void PWM0_3_Update_Duty_Cycle(uint16_t duty_cycle)
{
    // Set the duty cycle by writing to the COMPA field (Bits 15 to 0)
    PWM0->_3_CMPA = (duty_cycle - 1);
}
