#include "bsp_int.h"

/* 中断嵌套计数器 */
static unsigned int irqNesting;

//中断处理函数表
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

//初始化中断处理函数表

void system_irqtable_init(void)
{
    unsigned int i = 0;
    for (i=0; i < NUMBER_OF_INT_VECTORS; i++)
    {
        irqTable[i].irqHandler = default_irqhandler;
        irqTable[i].userParam = NULL;
    }
    
}

/*注册中断处理函数*/
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = userParam;
}

/* 中断初始化函数*/
void int_init(void)
{
    GIC_Init();
    system_irqtable_init();

    // 中断向量偏移设置
    __set_VBAR(0x87800000);

}

/*具体的中断处理函数。汇编的IRQ_Handler会调用它*/
void system_irqhandler(unsigned int giccIar)
{
    uint32_t intNum = giccIar & 0x3FFUL;

    /* 检查中断号是否符合要求 */
   if ((intNum == 1023) || (intNum >= NUMBER_OF_INT_VECTORS))
   {
	 	return;
   }

   irqNesting++;	/* 中断嵌套计数器加一 */

   /* 根据传递进来的中断号，在irqTable中调用确定的中断服务函数*/
   irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
 
   irqNesting--;	/* 中断执行完成，中断嵌套寄存器减一 */

}

/*默认中断处理函数*/
void default_irqhandler(unsigned int gicciar, void *userParam)
{
    while (1)
    {
    };
}