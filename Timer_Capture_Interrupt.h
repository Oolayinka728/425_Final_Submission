#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
void debug_LED(void);
void Timer3_Init(void);
void Trigger_Pulse(void);
void Timer3A_Handler(void);
float Calculate_Distance(uint32_t count);
uint32_t Timer3_Capture_Time(void);
void GPIO_Sensor_Init(void);