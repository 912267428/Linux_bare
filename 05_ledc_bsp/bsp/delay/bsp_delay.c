#include "bsp_delay.h"

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