#include <avr/io.h>
#include "led.h"


/** Turn LED1 on.  */
void led_on (void)
{
    /* Set port to turn LED 1 on.  */
    PORTC |= (1 << 2);
}


/** Turn LED1 off.  */
void led_off (void)
{
    /* Set port to turn LED 1 off.  */
    PORTC &= ~(1 << 2);
}


/** Initialise LED1.  */
void led_init (void)
{
    /* Initialise port to drive LED 1.  */
    DDRC |= (1 << 2);
}
