#include "bsp_epittimer.h"
#include "bsp_int.h"
#include "bsp_led.h"

void epit1_init(unsigned int frac, unsigned int ms)
{
    if (frac > 0xFFF)
        frac = 0xFFF;
    EPIT1->CR = 0;

    unsigned int value = ms * (66000000)/1000;
    /**
     * 配置EPIT1的CR寄存器
     * CLKSRC(bit25:24):选择EPIT的时钟源
     * PRESCALLAR（bit15:4）:设置分频值
     * RLD(bit3),设置工作模式: 1 当计数器到 0 的话从 LR 重新加载数
     * OCIEN(bit2): 1 比较中断使能
     * ENMOD(bit1): 1 初始计数值来源于 LR 寄存器值
    */
    EPIT1->CR = (1 << 24 | frac<<4 | 1 << 3 | 1 << 2 | 1 << 1);
    EPIT1->LR = value;  //加载寄存器的值 
    EPIT1->CMPR = 0;    //设在比较寄存器的值为0，所以减到0发生中断

    GIC_EnableIRQ(EPIT1_IRQn);

    system_register_irqhandler(EPIT1_IRQn,
                                (system_irq_handler_t)epit1_irqhandler,
                                NULL);
    
    EPIT1->CR |= 1 <<0;
}

void epit1_irqhandler(void)
{
    static unsigned char state = 0;
    if (EPIT1->SR & (1<<0))
    {
        state = !state;
        led_switch(LED0, state);
    }
    EPIT1->SR |= 1<<0;
    
}
