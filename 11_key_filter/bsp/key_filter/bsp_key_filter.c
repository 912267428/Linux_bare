#include "bsp_key_filter.h"
#include "bsp_int.h"
#include "bsp_gpio.h"
#include "bsp_beep.h"

/*初始化*/
void keyfilter_init(void)
{
    //初始化按键的IO，为中断
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, &key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);

    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_16_31_irqhandler, NULL);
    gpio_enableint(GPIO1, 18);

    //初始化定时器
    filtertimer_init(10);
}

/*初始化EPIT定时器*/
void filtertimer_init(unsigned int ms)
{
    unsigned int value = ms * (66000000)/1000;

    EPIT1->CR = 0;
    EPIT1->CR = (1 << 24 | 1 << 3 | 1 << 2 | 1 << 1);

    EPIT1->LR = value;
    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, filtertimer_irqhander, NULL);

}

/*关闭EPIT定时器*/
void filtertimer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

/*重启EPIT1定时器*/
void filtertimer_restart(unsigned int ms)
{
    unsigned int value = ms * (66000000)/1000;

    EPIT1->CR &= ~(1 << 0);
    EPIT1->LR = value;
    
    EPIT1->CR |= 1 << 0;
}

/*EPIT1定时器中断处理函数*/
void filtertimer_irqhander(unsigned int giccia, void *param)
{
    static unsigned char state = OFF;
    if (EPIT1->SR & (1 << 0))
    {
        filtertimer_stop();
        if(gpio_pinread(GPIO1, 18) == 0)
        {
            state = !state;
            beep_switch(state);
        }
    }
    
    /*清除中断标志位*/
    EPIT1->SR |= 1 << 0;
}

//按键中断服务函数
void gpio1_16_31_irqhandler(unsigned int giccia, void *param)
{
    /*开启定时器*/
    filtertimer_restart(10);
    /*清除中断标志位*/
    gpio_clearintflags(GPIO1, 18);
}