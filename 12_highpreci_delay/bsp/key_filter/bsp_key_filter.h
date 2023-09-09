#ifndef __BSP_KEYFILTER_H
#define  __BSP_KEYFILTER_H

#include "imx6ul.h"

void keyfilter_init(void);
void filtertimer_init(unsigned int ms);
void filtertimer_stop(void);
void filtertimer_restart(unsigned int ms);
void filtertimer_irqhander(unsigned int giccia, void *param);
void gpio1_16_31_irqhandler(unsigned int giccia, void *param);

#endif