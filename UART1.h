
#include "TM4C123GH6PM.h"

#define UART1_CR   0x0D
/**
 * @brief Line feed character
 */
#define UART1_LF   0x0A
/**
 * @brief Back space character
 */
#define UART1_BS   0x08
/**
 * @brief escape character
 */
#define UART1_ESC  0x1B
/**
 * @brief space character
 */
#define UART1SP   0x20
/**
 * @brief delete character
 */
#define UART1_DEL  0x7F

#define UART1_RECEIVE_FIFO_EMPTY_BIT_MASK 0x10
#define UART1_TRANSMIT_FIFO_FULL_BIT_MASK 0x20

void UART1_Output_Character(char data);
char UART1_Input_Character(void);
void UART1_Init(void);
void UART1_Input_String(char *buffer_pointer, uint16_t buffer_size);
void UART1_Output_String(char *pt);
void UART1_Output_Newline(void);
uint16_t MeasureDistance(void);
uint16_t Distance_UART(void);
