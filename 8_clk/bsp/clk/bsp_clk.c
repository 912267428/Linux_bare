#include "bsp_clk.h"


/**
 * @brief   :使能 I.MX6U 所有外设时钟
 * @param   :无
 * @retval  :无
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
 * @brief   初始化系统时钟为528Mhz，并设置PLL和PLL3各个PFD时钟，均设置为官方推荐值
 * @param   
 * @retval  
*/
void imux6u_clkinit(void)
{
    unsigned int reg = 0;
    //1、设置ARM内核时钟为528Mhz
    //1.1、判断当前使用那个时钟源源启动，正常使用pll1_sw_clk 驱动，但是pll1_sw_clk 有两个来源：
    //Pll1_main_clk 和 step_clk，如果要让 I.MX6ULL 跑到 528M，那必须选择 pll1_main_clk 作为 pll1 的时钟源。
    //如果我们要修改 pll1_main_clk 时钟的话就必须先将 pll1_sw_clk 从pll1_main_clk 切换到 step_clk,当修改完以后再将 pll1_sw_clk 切换
    //回 pll1_main_cl，step_clk 等于 24MHz。
    if ((((CCM->CCSR) >> 2) & 0x1) == 0)   //PLL1_main_clk
    {
        CCM->CCSR &= ~(1 << 8); //将CCM->CCSR寄存器的第8位置0，配置 step_clk 时钟源为 24MHz OSC
        CCM->CCSR |= (1 << 2);  //将CCM->CCSR寄存器的第2位置1,配置 pll1_sw_clk 时钟源为 step_clk
    }

    /**
     * 1.2、设置 pll1_main_clk 为 1056MHz,也就是 528*2=1056MHZ
     * 配置 CCM_ANLOG->PLL_ARM 寄存器
     * bit13: 1 使能时钟输出
     * bit[6:0]: 88, 由公式：Fout = Fin * div_select / 2.0，
     * 1056=24*div_select/2.0, 得出：div_select=88。
    */
   CCM_ANALOG->PLL_ARM = (1 << 13) | ((88 << 0) & 0x7F);
   CCM->CCSR &= ~(1 << 2); //将CCM->CCSR寄存器的第2位置0,配置 pll1_sw_clk 时钟源为 PLL1_main_clk
   CCM->CACRR = 1;

   /**
    * 2、设置PLL2（SYS PLL）各个PFD
   */
    reg = CCM_ANALOG->PFD_528;
    reg &= ~(0x3F3F3F3F);
    reg |=32 << 24; //528*18/297
    reg |=24 << 16; //528*18/396
    reg |=16 << 8;  //528*18/594
    reg |=27 << 0;  //528*18/352
    CCM_ANALOG->PFD_528 = reg;

  /**
   * 设置 PLL3(USB1)各个 PFD
  */
    reg = 0;
    reg = CCM_ANALOG->PFD_480;
    reg &= ~(0X3F3F3F3F); // 清除原来的设置
    reg |=19 << 24; //528*18/454.7
    reg |=17 << 16; //528*18/508.2
    reg |=16 << 8;  //528*18/540
    reg |=12 << 0;  //528*18/720
    CCM_ANALOG->PFD_480 = reg;

    /**
     * 设置AHB时钟 最小6Mhz,最大132Mhz
    */
   CCM->CBCMR &= ~(3 << 18);    //操作CCM_CBCMR的PRE_PERIPH_CLK_SEL来选择PLL2_PDF2
   CCM->CBCMR |= (1 << 18);     //操作CCM_CBCMR的PRE_PERIPH_CLK_SEL来选择PLL2_PDF2
   CCM->CBCDR &= ~(1 << 25);
   while(CCM->CDHIPR & (1 << 5)); //等到握手完成

#if 0
    //先关闭AHB_Root_CLK输出，否则时钟设置出错
    CCM->CBCDR &= ~(7 << 10);
    CCM->CBCDR |= 2 << 10;
    while(CCM->CDHIPR & (1 << 1));
#endif // 0

    /**
     * 5、设置IPG_CLK_ROOT最小3Mhz，最大66Mhz
    */
   CCM->CBCDR &= ~(3 << 8);
   CCM->CBCDR |= 1 << 8;

   /**
    * 6、设置 PERCLK_CLK_ROOT 时钟
   */
    CCM->CSCMR1 &= ~(1 << 6);
    CCM->CSCMR1 &= ~(7 << 0);
}