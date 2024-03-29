#include "imx6ul.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"

int main(void)
{
    int i =0;
    int keyvalue = 0;
    unsigned char led_state = OFF;
    unsigned char beep_state = OFF;

    clk_enable();
    led_init();
    beep_init();
    key_init();

    while (1)
    {
        keyvalue = key_getvalue();
        if (keyvalue)
        {
            switch (keyvalue)
            {
            case KEY0_VALUE:
                beep_state = !beep_state;
                beep_switch(beep_state);
                break;
            default:
                break;
            }
        }
        i++;
        if(i==50)
        {
            i = 0;
            led_state = !led_state;
            led_switch(LED0, led_state);
        }
        delay(10);
    }
    
}