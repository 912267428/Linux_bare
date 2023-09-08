#include "imx6ul.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "bsp_epittimer.h"

int main(void)
{
    unsigned char led_state = OFF;

    int_init();
    imx6u_clkinit();
    clk_enable();
    led_init();
    beep_init();
    key_init();
    exit_init();
    epit1_init(0, 50);

    while (1)
    {
        delay(500);
    }
    
}
