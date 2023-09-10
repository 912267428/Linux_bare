#include "imx6ul.h"
#include "bsp.h"
#include "stdio.h"

int main(void)
{
    int a,b;
    unsigned char state = OFF;

    int_init();
    imx6u_clkinit();
    delay_init();
    clk_enable();

    led_init();
    beep_init();
    uart_init();


    while (1)
    {
        printf("输入两个整数，用空格隔开：");
        scanf("%d %d", &a, &b);
        printf("\r\n数据%d + %d = %d\r\n\r\n", a, b, a+b);

        state = !state;
        led_switch(LED0, state);
    }
    
}
