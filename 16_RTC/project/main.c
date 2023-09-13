#include "imx6ul.h"
#include "bsp.h"
#include "stdio.h"

int main(void)
{
    unsigned char key=0;
    int i=3,t=0;
    char buf[160];
	struct rtc_datetime rtcdate;
    unsigned char state = OFF;

    int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	delay_init();				/* 初始化延时 			*/
	clk_enable();				/* 使能所有的时钟 			*/

	key_init();
	led_init();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 			*/
	rtc_init(); 				/* 初始化RTC	 		*/

    tftlcd_dev.forecolor = LCD_RED;
    lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6UL RTC TEST");    /* 显示字符串 */
	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");  
	lcd_show_string(50, 60, 200, 16, 16, (char*)"2019/3/27");  
	tftlcd_dev.forecolor = LCD_BLUE;
	memset(buf, 0, sizeof(buf));
    
    while (1)
    {
        if (t == 100)
        {
            t=0;
            printf("will be running %d s.......", i);

            lcd_fill(50, 90, 370, 110, tftlcd_dev.backcolor);
            sprintf(buf, "will be running %ds......", i);
            lcd_show_string(50, 90, 300, 16, 16, buf); 
            i--;
            if (i<0)
            {
                break;
            }
        }

        key = key_getvalue();
        if(key == KEY0_VALUE)
		{
			rtcdate.year = 2023;
   			rtcdate.month = 9;
    		rtcdate.day = 13;
    		rtcdate.hour = 21;
    		rtcdate.minute = 57;
    		rtcdate.second = 0;
			rtc_setdatetime(&rtcdate); /* 初始化时间和日期 */
			printf("\r\n RTC Init finish\r\n");
			break;
		}
        delay_ms(10);
        t++;
    }
    tftlcd_dev.forecolor = LCD_RED;
	lcd_fill(50, 90, 370, 110, tftlcd_dev.backcolor); /* 清屏 */
	lcd_show_string(50, 90, 200, 16, 16, (char*)"Current Time:");  			/* 显示字符串 */
	tftlcd_dev.forecolor = LCD_BLUE;

    while(1)					
	{	
		rtc_getdatetime(&rtcdate);
		sprintf(buf,"%d/%d/%d %d:%d:%d",rtcdate.year, rtcdate.month, rtcdate.day, rtcdate.hour, rtcdate.minute, rtcdate.second);
		lcd_fill(50,110, 300,130, tftlcd_dev.backcolor);
		lcd_show_string(50, 110, 250, 16, 16,(char*)buf);  /* 显示字符串 */
		
		state = !state;
		led_switch(LED0,state);
		delay_ms(1000);	/* 延时一秒 */
	}
    
}
