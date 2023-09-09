#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "imx6ul.h"

void delay_init(void);
void delay_us(unsigned int usdelay);
void delay_ms(unsigned int msdelay);
void gpt1_irqhandle(void);

void delay(volatile unsigned int n);

#endif
