#include "bsp_led.h"

/**
 * @brief   :初始化 LED 对应的 GPIO
 * @param   :无
 * @retval  :无
*/
void led_init(void)
{
    //初始化IO复用
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);

    //配置 GPIO1_IO03 的 IO 属性
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);

    //初始化 GPIO,GPIO1_IO03 设置为输出
    GPIO1->GDIR |= (1 << 3);

    //设置 GPIO1_IO03 输出低电平，打开 LED0
    GPIO1->DR &= ~(1 << 3);
}

/**
 * @brief   LED 控制函数，控制 LED 打开还是关闭
 * @param led   要控制的 LED 灯编号
 * @param status 0，关闭 LED0，1 打开 LED0
 * @retval  无
*/
void led_switch(int led, int status)
{
    switch (led)
    {
    case LED0:
        if (status == ON)
            GPIO1->DR &= ~(1<<3);
        else if (status == OFF)
            GPIO1->DR |= (1<<3);  
        break;
        
    default:
        break;
    }
}