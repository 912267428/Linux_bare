#include "bsp_beep.h"

/**
 * @brief 初始化蜂鸣器对应的IO
 * @param 无
 * @retval 无
*/
void beep_init(void)
{
    //1、初始化IO复用
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);

    //2、配置IO属性
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10B0);

    GPIO5->GDIR |= (1 << 1);

    GPIO5->DR |= (1 << 1);
}

/**
 * @brief 蜂鸣器控制函数，控制蜂鸣器打开还是关闭
 * @param status  0，关闭蜂鸣器，1 打开蜂鸣器
 * @retval 无
*/
void beep_switch(int status)
{
    if (status == ON)
    {
        GPIO5->DR &= ~(1 << 1);
    }else if (status == OFF)
    {
        GPIO5->DR |= (1 << 1);
    }
        
}