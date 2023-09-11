#include "imx6ul.h"
#include "bsp.h"
#include "stdio.h"

unsigned int backcolor[10] = {
    LCD_BLUE, LCD_GREEN, LCD_RED, LCD_CYAN, LCD_YELLOW,
    LCD_LIGHTBLUE, LCD_DARKBLUE, LCD_WHITE, LCD_BLACK, LCD_ORANGE};

int main(void)
{
    unsigned char index = 0;
    unsigned char state = OFF;

    int_init();
    imx6u_clkinit();
    delay_init();  
    clk_enable();
    led_init();
    beep_init();
    uart_init();
    lcd_init();

    tftlcd_dev.forecolor = LCD_RED;
    lcd_show_string(10,10,400,32,32,(char*)"IMX6ULL TEST");
    lcd_draw_rectangle(10, 52, 500, 200); /* 绘制矩形框 */
    lcd_drawline(10, 52,700, 300); /* 绘制线条 */
    lcd_drawline(10, 290,700, 300); /* 绘制线条 */
    lcd_draw_Circle(200, 400, 100); /* 绘制圆形 */

    while (1)
    {
        index++;
        if(index == 10) index = 0;
        lcd_fill(0, 210, 799, 479, backcolor[index]);
        lcd_show_string(600,10,100,32,32,(char*)"INDEX=");
        lcd_shownum(696,10, index, 2, 32); /* 显示数字，叠加显示 */

        state = !state;
        led_switch(LED0,state);
        delay_ms(100);
    }
    
}
