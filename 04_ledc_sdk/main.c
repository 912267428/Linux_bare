#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

/**
 * @brief 使能所有外设时钟
 * @param   无
 * @return  无
*/
void clk_enable(void)
{
    CCM->CCGR0 = 0XFFFFFFFF;
    CCM->CCGR1 = 0XFFFFFFFF;
    CCM->CCGR2 = 0XFFFFFFFF;
    CCM->CCGR3 = 0XFFFFFFFF;
    CCM->CCGR4 = 0XFFFFFFFF;
    CCM->CCGR5 = 0XFFFFFFFF;
    CCM->CCGR6 = 0XFFFFFFFF;
}


/**
 * @brief 初始化 LED 对应的 GPIO
 * @param   无
 * @return  无
*/
void led_init(void)
{
    //初始化IO复用
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);

    /* 2、、配置GPIO1_IO03的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);

    //初始化GPIO
    GPIO1->GDIR |= (1 << 3);

    //设置GPIO1_IO03输出低电平，打开LED0
    GPIO1->DR &= ~(1 << 3);
}

/**
 * @brief 打开LED
 * @param   无
 * @return  无
*/
void led_on(void)
{
    //将 GPIO1_DR 的 bit3 清零
    GPIO1->DR &= ~(1 << 3);
}

/**
 * @brief 关闭LED
 * @param   无
 * @return  无
*/
void led_off(void)
{
    //将 GPIO1_DR 的 bit3 置一
    GPIO1->DR |= (1 << 3);
}

/*
 * @description	: 短时间延时函数
 * @param - n	: 要延时循环次数(空操作循环次数，模式延时)
 * @return 		: 无
 */
void delay_short(volatile unsigned int n)
{
	while(n--){}
}

/*
 * @description	: 延时函数,在396Mhz的主频下
 * 			  	  延时时间大约为1ms
 * @param - n	: 要延时的ms数
 * @return 		: 无
 */
void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}

int main(void)
{
    clk_enable();
    led_init();

    while (1)
    {
        led_off();
        delay(300);

        led_on();
        delay(100);
    }
    
}