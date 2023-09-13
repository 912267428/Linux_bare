#include "bsp_delay.h"
#include "bsp_led.h"


/**
 * @brief   短时间延时函数
 * @param n 要延时循环次数(空操作循环次数，模式延时)
 * @retval  
*/
void delay_shot(volatile unsigned int n)
{
    while (n--)
    {
    } 
}

/**
 * @brief   延时函数,在 396Mhz 的主频下延时时间大约为 1ms
 * @param n 要延时的 ms 数
 * @retval  
*/
void delay(volatile unsigned int n)
{
    while (n--)
    {
        delay_shot(0x7ff);
    }
    
}

void delay_init(void)
{
    GPT1->CR = 0;
    GPT1->CR |= (1 << 15);  //bit15置1，进入软复位
    while ((GPT1->CR >> 15) & 0x01); //等待完成复位

    /**配置GPT1的CR寄存器
     * bit22:20 000 输出比较1的输出功能关闭
     * bit9 0   Restart 模式,当 CNT 等于 OCR1 的时候就产生中断
     * bit8:6 001 GPT 时钟源选择 ipg_clk=66Mhz
    */
    GPT1->CR = (1<<6);

   /**
    * 配置GPT1的PR寄存器，设置GPT 的分频设置
    * bit11:0 设置分频值，设置为 0 表示 1 分频，
    *   以此类推，最大可以设置为 0XFFF，也就是最大 4096 分频
   */
    GPT1->PR = 65;

  /**
   * 配置GPT1的OCR1寄存器，设置GPT 的输出比较 1 比较计数值
   * GPT 的时钟为 1Mz，那么计数器每计一个值就是就是 1us。
   * 这里设置为最大值
  */
    GPT1->OCR[1] = 0xFFFFFFFF;
    GPT1->CR |= 1 << 0;   //使能GPT1

    /* 以下屏蔽的代码是GPT定时器中断代码，
	 * 如果想学习GPT定时器的话可以参考一下代码。
	 */
#ifdef USE_GPT_IRQ
	/*
     * GPT的PR寄存器，GPT的分频设置
     * bit11:0  设置分频值，设置为0表示1分频，
     *          以此类推，最大可以设置为0XFFF，也就是最大4096分频
	 */
	GPT1->PR = 65;	//设置为1，即65+1=66分频，因此GPT1时钟为66M/66=1MHz


	 /*
      * GPT的OCR1寄存器，GPT的输出比较1比较计数值，
      * 当GPT的计数值等于OCR1里面值时候，输出比较1就会发生中断
      * 这里定时500ms产生中断，因此就应该为1000000/2=500000;
	  */
	GPT1->OCR[0] = 500000;

	/*
     * GPT的IR寄存器，使能通道1的比较中断
     * bit0： 0 使能输出比较中断
	 */
	GPT1->IR |= 1 << 0;

	/*
     * 使能GIC里面相应的中断，并且注册中断处理函数
	 */
	GIC_EnableIRQ(GPT1_IRQn);	//使能GIC中对应的中断
	system_register_irqhandler(GPT1_IRQn, (system_irq_handler_t)gpt1_irqhandler, NULL);	//注册中断服务函数	
#endif
}

#ifdef USE_GPT_IRQ
void gpt1_irqhandle(void)
{
    static unsigned char state = 0;
    state = !state;

    if(GPT1->SR & (1<<0))
    {
        led_switch(LED2, state);
    }

    GPT1->SR |= 1 << 0;
}
#endif

/**
 * @brief us微妙级延时
 * @param usdelay 需要延时的us数目
 * @retval 无
*/
void delay_us(unsigned int usdelay)
{
    unsigned long oldcnt, newcnt;
    unsigned long tcntvalue = 0;
    oldcnt = GPT1->CNT;

    while (1)
    {
        newcnt = GPT1->CNT;
        if(newcnt != oldcnt)
        {
            if(newcnt > oldcnt)  //没有溢出
                tcntvalue += newcnt - oldcnt;
            else                 //发生溢出
                tcntvalue += 0xFFFFFFFF - oldcnt + newcnt;
            oldcnt = newcnt;
            if (tcntvalue >= usdelay)
                break;
            
        }
    }
}

/**
 * @brief ms毫秒级延时
 * @param msdelay 需要延时的ms毫秒数目
 * @retval 无
*/
void delay_ms(unsigned int msdelay)
{
    int i = 0;
    for (i = 0; i < msdelay; i++)
    {
        delay_us(1000);
    }       
}