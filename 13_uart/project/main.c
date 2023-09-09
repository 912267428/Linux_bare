#include "imx6ul.h"
#include "bsp.h"

int main(void)
{
    unsigned char a = 0;
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
        puts("请输入一个字符:");
        a = getc();
        putc(a);
        puts("\r\n");

        puts("您输入的字符为：");
        putc(a);
        puts("\r\n\r\n");

        state = !state;
        led_switch(LED0, state);
    }
    
}
