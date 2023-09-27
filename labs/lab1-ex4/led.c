#include "pio.h"
#include "led.h"
#define LED_PIO PC2_PIO

/** Turn LED1 on.  */
void led_on (void)
{
    pio_output_high (LED_PIO);
}


/** Turn LED1 off.  */
void led_off (void)
{
    pio_output_low (LED_PIO);
}


/** Initialise LED1.  */
void led_init (void)
{
    pio_config_set (LED_PIO, PIO_OUTPUT_LOW);
}
