
#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "GPIO.h"

// Constant definitions for the buzzer
extern const uint8_t BUZZER_OFF;
extern const uint8_t BUZZER_ON;

// Constant definitions for musical notes
extern const double C4_NOTE;
extern const double D4_NOTE;
extern const double E4_NOTE;
extern const double F4_NOTE;
extern const double G4_NOTE;
extern const double A4_NOTE;
extern const double B4_NOTE;
extern const double A5_NOTE;
extern const double G5_NOTE;
extern const double C5_NOTE;

void Buzzer_Init(void);
void Note_Controller(uint8_t push_button);
void Play_Note(double note, unsigned int duration);
void Buzzer_Output (uint8_t buzzer_value);
void alarm(void);