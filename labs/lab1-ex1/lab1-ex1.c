#include <avr/io.h>
#include "system.h"

int main (void)
{
    system_init ();

    DDRC |= (1 << 2);
    DDRD &= ~(1 << 7);

    while (1)
    {        
        if ((PIND & (1 << 7)) != 0) {
            PORTC |= (1 << 2);
        } else {
            PORTC &= ~(1 << 2);
        }
    }
}
