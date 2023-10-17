/**  @file   logic.h
     @author Harry Ellis & Sean Gouw
     @date   October 2023
     @brief  Logic of changing letter on LCD & Game States
*/


#include "logic.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "led.h"


// Increments the letter on the LCD when the navagation swtich gets pushed north
void increment_letter(void) 
{
    if (currentIndex < maxpush) {
        
        if (myturn && currentIndex < 25) { 
            currentIndex++;
            tinygl_text(game_letter[currentIndex]);
        }
    }
}


// Decrements the letter on the LCD when the navagation swtich gets pushed south
void decrement_letter(void)
{
    if (myturn && currentIndex > maxpush - 2) {
        currentIndex--;
        tinygl_text(game_letter[currentIndex]);
    }
}


// Only called after a game is complete to the same job as setup_game() without needing an input to start again
void reset_game(void)
{
    maxpush = 3;
    currentIndex = 0;
    state = START_ROUND;
    tinygl_clear();
    tinygl_text(game_letter[currentIndex]);
    ir_uart_putc('B');
}


// Gets called in the intial game setup after clicking inital navigation switch push down
// One player pushes navigation switch push down to start the game, this sends a message through IR to start other players game too
void setup_game(void)
{
    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        maxpush = 3;
        currentIndex = 0;
        state = START_ROUND;
        tinygl_clear();
        tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
        tinygl_text(game_letter[currentIndex]);
        ir_uart_putc('S');
    }

    if (ir_uart_read_ready_p()) {
        if (ir_uart_getc() == 'S') {
            tinygl_clear();
            led_set(LED1, 0);
            state = START_ROUND;
            myturn = 0;
        }
    }
}