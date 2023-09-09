#include "bsp_gpio.h"

/**
 * @brief GPIO 初始化。
 * @param base 要初始化的 GPIO 组。
 * @param pin 要初始化 GPIO 在组内的编号。
 * @param config GPIO 配置结构体。
 * @retval 无
*/
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    if (config->direction == kGPIO_DigitalInput)
    {
        base->GDIR &= ~(1 << pin);
    }else
    {
        base->GDIR |= 1 << pin;
        gpio_pinwrite(base, pin, config->outputLogic);    //输出默认电平
    }
    gpio_intconfig(base, pin, config->interruptMode);	/* 中断功能配置 */
}

/**
 * @brief 读取指定 GPIO 的电平值 。
 * @param base 要读取的 GPIO 组。
 * @param pin 要读取的 GPIO 脚号。
 * @retval 读取的值
*/
int gpio_pinread(GPIO_Type *base, int pin)
{
    return (((base->DR) >> pin ) & 0x1 );
}

/**
 * @brief 指定 GPIO 输出高或者低电平 。
 * @param base 要输出的的 GPIO 组。
 * @param pin 要输出GPIO 在组内的编号。
 * @param value 要输出的电平，1 输出高电平， 0 输出低低电平
 * @retval 无
*/
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
    if (value == 0U)
    {
        base->DR &= ~(1U << pin);
    }else
    {
        base->DR |= (1U << pin);
    }    
}

/**使能指定IO中断*/
void gpio_enableint(GPIO_Type* base, unsigned int pin)
{
    base->IMR |= (1 << pin);
}

/**禁止指定IO中断*/
void gpio_disableint(GPIO_Type* base, unsigned int pin)
{
    base->IMR &= ~(1 << pin);
}

/*
 * @description  			: 清除中断标志位(写1清除)
 * @param - base 			: 要清除的IO所在的GPIO组。
 * @param - pin  			: 要清除的GPIO掩码。
 * @return		 			: 无
 */
void gpio_clearintflags(GPIO_Type* base, unsigned int pin)
{
    base->ISR |= (1 << pin);
}

/*
 * @description  			: 设置GPIO的中断配置功能
 * @param - base 			: 要配置的IO所在的GPIO组。
 * @param - pin  			: 要配置的GPIO脚号。
 * @param - pinInterruptMode: 中断模式，参考枚举类型gpio_interrupt_mode_t
 * @return		 			: 无
 */
void gpio_intconfig(GPIO_Type* base, unsigned int pin, gpio_interrupt_mode_t pinInterruptMode)
{
    volatile uint32_t *icr;
	uint32_t icrShift;

    icrShift = pin;
	
	base->EDGE_SEL &= ~(1U << pin);

    if (pin < 16)
    {
        icr = &(base->ICR1);
    }else{
        icr = &(base->ICR2);
        icrShift -= 16;
    }
    
    switch (pinInterruptMode)
    {
    case (kGPIO_IntLowLevel):
        *icr &= ~(3U << (2 * icrShift));
        break;
    case(kGPIO_IntHighLevel):
		*icr = (*icr & (~(3U << (2 * icrShift)))) | (1U << (2 * icrShift));
		break;
	case(kGPIO_IntRisingEdge):
		*icr = (*icr & (~(3U << (2 * icrShift)))) | (2U << (2 * icrShift));
		break;
	case(kGPIO_IntFallingEdge):
		*icr |= (3U << (2 * icrShift));
		break;
	case(kGPIO_IntRisingOrFallingEdge):
		base->EDGE_SEL |= (1U << pin);
		break;    
    
    default:
        break;
    }

}