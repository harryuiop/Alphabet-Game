#include "pio.h"
#include "button.h"
#define BUTTON1_PIO PD7_PIO


/** Return non-zero if button pressed.  */
int button_pressed_p (void)
{   
    bool pushed = pio_input_get(BUTTON1_PIO);
    return pushed;
}


/** Initialise button1.  */
void button_init (void)
{
    pio_config_set (BUTTON1_PIO, PIO_INPUT);
}

