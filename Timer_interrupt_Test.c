#include "TM4C123GH6PM.h"



void Timer3ACapture_init(void)
{
    SYSCTL->RCGCTIMER |= (1 << 3);    /* enable clock to Timer Block 3 */
    SYSCTL->RCGCGPIO |= (1 << 1);     /* enable clock to PORTB (PB2) */
    SYSCTL->RCGCGPIO |= (1 << 4);     /* enable clock to PORTE (PE0) */
    
    /* Configure PB2 (Echo) as input for Timer3 Capture */
    GPIOB->DIR &= ~0x04;              /* make PB2 an input pin */
    GPIOB->DEN |= 0x04;               /* make PB2 a digital pin */
    GPIOB->AFSEL |= 0x04;             /* use PB2 alternate function */
    GPIOB->PCTL &= ~0x00000F00;       /* configure PB2 for T3CCP0 */
    GPIOB->PCTL |= 0x00000700;
    
    /* Configure PE0 (Trigger) as digital output */
    GPIOE->DIR |= 0x01;               /* set PE0 as a digital output pin */
    GPIOE->DEN |= 0x01;               /* make PE0 a digital pin */

    /* Configure Timer3A for edge-time capture mode */
    TIMER3->CTL &= ~1;                /* disable timer3A during setup */
    TIMER3->CFG = 4;                  /* 16-bit timer mode */
    TIMER3->TAMR = 0x17;              /* up-count, edge-time, capture mode */
    TIMER3->CTL |= 0x0C;              /* capture the rising edge */
    TIMER3->CTL |= (1 << 0);          /* enable timer3A */
}

void Delay_MicroSecond(int time)
{
    int i;
    SYSCTL->RCGCTIMER |= (1 << 3);   /* enable clock to Timer Block 3 */
    TIMER3->CTL = 0;                  /* disable Timer before initialization */
    TIMER3->CFG = 0x04;               /* 16-bit option */
    TIMER3->TAMR = 0x02;              /* periodic mode and down-counter */
    TIMER3->TAILR = 16 - 1;           /* TimerA interval load value reg (16 MHz clock gives 1 us delay) */
    TIMER3->ICR = 0x1;                /* clear the TimerA timeout flag */
    TIMER3->CTL |= 0x01;              /* enable Timer A after initialization */

    for(i = 0; i < time; i++)
    {
        while ((TIMER3->RIS & 0x1) == 0); /* wait for TimerA timeout flag */
        TIMER3->ICR = 0x1;              /* clear the TimerA timeout flag */
    }
}


uint32_t Measure_distance(void)
{
    int lastEdge, thisEdge;

    /* Given 10us trigger pulse on PE0 */
    GPIOE->DATA &= ~(1<<0);   /* make trigger pin (PE0) low */
    Delay_MicroSecond(10);     /* 10 microseconds delay */
    GPIOE->DATA |= (1<<0);    /* make trigger pin (PE0) high */
    Delay_MicroSecond(10);     /* 10 microseconds delay */
    GPIOE->DATA &= ~(1<<0);   /* make trigger pin (PE0) low */

    while(1)
    {
        TIMER3->ICR = 4;              /* clear Timer 3A capture flag */
        while((TIMER3->RIS & 4) == 0);/* wait till captured */
        
        if(GPIOB->DATA & (1<<2))      /* check if rising edge occurs on PB2 (Echo) */
        {
            lastEdge = TIMER3->TAR;  /* save the timestamp for rising edge */
            
            /* detect falling edge */
            TIMER3->ICR = 4;          /* clear Timer 3A capture flag */
            while((TIMER3->RIS & 4) == 0);  /* wait till captured */
            
            thisEdge = TIMER3->TAR;  /* save the timestamp for falling edge */
            return (thisEdge - lastEdge); /* return the time difference */
        }
    }
}
