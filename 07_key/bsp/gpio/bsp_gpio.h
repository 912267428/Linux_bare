#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "imx6ul.h"

typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput = 1U,
}gpio_pin_direction_t;

/* GPIO 配置结构体 */
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction; //方向
    uint8_t outputLogic;            //输出模式下的默认输出电平
} gpio_pin_config_t;


void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);


#endif // ! __BSP_GPIO_H