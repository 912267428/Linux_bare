#include "bsp_uart.h"

void uart_init(void)
{
    /**
     * 1、初始化串口IO
     *  UART1_RXD 复用为 UART1_RXD
     *  UART1_TXD 复用为 UART1_TXD
     *  电气化属性配置为0x10B0
    */
    uart_io_init();

    /**
     * 2、初始化UART1
    */
    uart_disable(UART1);
    uart_softreset(UART1);

    UART1->UCR1 = 0;
    UART1->UCR1 &= ~(1 << 14);

    /**
     * 设置 UART 的 UCR2 寄存器，设置字长，停止位，校验模式，关闭硬件流控
     * bit14,IRTS  1    忽略RTS
     * bit8，PREN   0   关闭奇偶校验
     * bit6，STOP   0   1位停止位
     * bit5，WS     1   8位数据位
     * bit2：TXEN   1   打开发送
     * bit1：RXEN   1   打开接收
    */
    UART1->UCR2 |= (1<<14) | (1<<5) | (1<<2) | (1<<1);
    UART1->UCR3 |= 1<<2;

    /**
     * 设置波特率
     * 计算公式:Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1))
     * Ref Freq = 80M   是寄存器 UFCR 的 bit9:7=101, 表示 1 分频
     * UBMR = 3124
     * UBIR = 71
     * 波特率= 80000000/(16 * (3124+1)/(71+1))=(80000000*72) / (16*3125)=115200
    */
    UART1->UFCR = 5<<7; //设置为1分频 ref freq 等于 ipg_clk/1=80Mhz
	UART1->UBIR = 71;
	UART1->UBMR = 3124;

#ifdef SETBAUDRATE
    uart_setbaudrate(UART1, 115200, 80000000);
#endif

    uart_enable(UART1); 
}

void uart_io_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);

}

/*
 * @description 		: 波特率计算公式，
 *    			  	  	  可以用此函数计算出指定串口对应的UFCR，
 * 				          UBIR和UBMR这三个寄存器的值
 * @param - base		: 要计算的串口。
 * @param - baudrate	: 要使用的波特率。
 * @param - srcclock_hz	:串口时钟源频率，单位Hz
 * @return		: 无
 */
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;		//分子
    uint32_t denominator = 0U;		//分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); //要先写UBIR寄存器，然后在写UBMR寄存器，3592页 
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}

void uart_disable(UART_Type *base)
{
    base->UCR1 &= ~(1 << 0);
}

void uart_enable(UART_Type *base)
{
    base->UCR1 |= (1<<0);
}

void uart_softreset(UART_Type *base)
{
    base->UCR2 &= ~(1 << 0);
    while((base->UCR2 & 0x1) == 0);
}

/**
 * @brief 发送一个字符
 * @param c 要发送的字符
 * @retval 无
*/
void putc(unsigned char c)
{
    while(((UART1->USR2 >> 3) & 0x01) == 0);
    UART1->UTXD = c & 0xFF;
}

/**
 * @brief 发送一个字符串
 * @param str 要发送的字符串
 * @retval 无
*/
void puts(char *str)
{
    char *p = str;
    
    while (*p)
    {
        putc(*p++);
    }
    
}

/**
 * @brief 接收一个字符
 * @param 无
 * @retval 接收到的字符串
*/
unsigned char getc(void)
{
    while ((UART1->USR2 & 0x1) == 0);
    return UART1->URXD;
}

/**
 * @brief 接收一个字符串
 * @param 接收字符串的数组首地址
 * @retval 无
*/
void gets(char *str)
{
    unsigned char c;
    char *p = str;
    do
    {
        c = getc();
        *p++ = c;
    } while (c != '#');

    *p = '\0';
    
}

void raise(int sig_nr)
{

}