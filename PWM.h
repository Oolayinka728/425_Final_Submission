
#include "TM4C123GH6PM.h"
void PWM0_3_Init(uint16_t period_constant, uint16_t duty_cycle);

/**
 * @brief Updates the PWM Module 0 Generator 0 duty cycle for the PWM signal on the PB6 pin (M0PWM0).
 *
 * @param duty_cycle The new duty cycle for the PWM signal on the PB6 pin (M0PWM0).
 *
 * @return None
 */
void PWM0_3_Update_Duty_Cycle(uint16_t duty_cycle);