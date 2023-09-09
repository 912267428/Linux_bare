#include "imx6ul.h"
#include "bsp.h"
#define STRLEN_MAX 100

char is_equal(char *s1, char *s2)
{
    char *p1=s1;
    char *p2=s2;
    while (p1 && p2)
    {
        if (*p1 != *p2)
            break;
        if(*p1 == '\0' && *p2 == '\0')
            return 0;
        p1++;
        p2++;
    }
    
    
    return 1;
}

int main(void)
{
    char str[STRLEN_MAX];
    unsigned char state = OFF;

    int_init();
    imx6u_clkinit();
    delay_init();
    clk_enable();

    led_init();
    beep_init();
    uart_init();

    led_switch(LED0, state);
    while (1)
    {
        puts("输入一个指令:");
        
        gets(str);
        puts(str);
        puts("\r\n");

        if(is_equal(str, "open_beep#") == 0)
        {
            puts("收到指令，即将打开蜂鸣器\r\n\r\n");
            beep_switch(ON);
            continue;
        }
        if(is_equal(str, "close_beep#") == 0)
        {
            puts("收到指令，即将关闭蜂鸣器\r\n\r\n");
            beep_switch(OFF);
            continue;
        }

        if(is_equal(str, "open_led#") == 0)
        {
            puts("收到指令，即将打开led灯\r\n\r\n");
            led_switch(LED0, ON);
            continue;
        }
        if(is_equal(str, "close_led#") == 0)
        {
            puts("收到指令，即将关闭led灯\r\n\r\n");
            led_switch(LED0, OFF);
            continue;
        }

    }
    
}
