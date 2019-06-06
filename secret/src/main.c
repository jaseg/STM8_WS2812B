#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "stm8s_conf.h"
#include "stm8s_tim2.h"

#define UNUSED(x) ((void)(x))

void _delay_ms(uint16_t ms);
void _delay_us(uint16_t us);

void main(void) {  
    uint8_t inputs;
    uint8_t i, idx;

#define LOG_LEN 32
    uint8_t input_log[LOG_LEN];
    uint16_t input_times[LOG_LEN];
    uint8_t log_idx = 0;

#define MAX_IGNORED_BLIP_TIME 20 /* 10ms increments */
#define PATTERN_LEN 4
    uint8_t event_pattern_log[PATTERN_LEN]   = { 0, 1, 2, 0 }; /* Ordered last item to first item! */
    uint8_t event_pattern_min_times[PATTERN_LEN] = { 5, 5, 5, 5 }; /* In 10ms increments */
    uint8_t event_pattern_max_times[PATTERN_LEN] = { 200, 200, 200, 200 }; /* In 10ms increments */

    uint8_t matched = 0;
#define EVENT_COUNT_TRIGGER 2
    uint8_t event_count = 0;
#define EVENT_TIMEOUT 1000 /* 10ms increments */
    uint16_t event_timer = 0;

#define OUTPUT_TIMEOUT 300 /* 10ms increments */
    uint16_t output_timer = 0;

    CLK->CKDIVR = 0x00;
    /* FIXME remove this and the timer code below if unneeded
    CLK->PCKENR1 |= 0x20;
    */

    GPIOB->DDR |= (1<<5); /* "test" LED */
    GPIOB->ODR |= (1<<5); /* "test" LED */
    GPIOD->DDR |= (1<<4); /* nitinol wire output */
    GPIOD->ODR |= (1<<4); /* Disable wire (output is active low) */

    for (i=0; i<LOG_LEN; i++) { /* We're re-using i on purpose here. */
        input_log[i] = 0xff;
        input_times[i] = 0xffff;
    }

    /*
    TIM2->CR1   =  0x01;
    TIM2->CCMR3 =  0x60;
    TIM2->CCER2 =  0x01;
    TIM2->ARRH  =  0x01;
    TIM2->ARRL  =  0x00;
    */

    while(1) { 
        _delay_ms(50);
        /* run every 10ms */
        matched = 0;
        for (i=0; i<LOG_LEN; i++) {
            idx = (LOG_LEN + log_idx - i) % LOG_LEN;

            /* Check conditions: State, and time upper and lower bounds */
            if (input_log[idx] == event_pattern_log[matched] &&
                input_times[idx] >= event_pattern_min_times[matched] &&
                input_times[idx] <= event_pattern_max_times[matched]) {

                matched++;
                if (matched < PATTERN_LEN)
                    continue; /* Not yet... */

                /* Match success */
                /* Clear matching log */
                for (i=0; i<LOG_LEN; i++) { /* We're re-using i on purpose here. */
                    input_log[i] = 0;
                    input_times[i] = 0;
                }
                event_count++;
                event_timer = EVENT_TIMEOUT;
                if (event_count < EVENT_COUNT_TRIGGER)
                    break; /* Not yet... */

                /* Event count trigger reached */
                /* Reset matching machinery */
                event_count = 0;
                event_timer = 0;

                /* Activate nitinol wire actuator to open latch */
                GPIOD->ODR &= ~(1<<4);
                output_timer = OUTPUT_TIMEOUT;
                break;
            }

            /* Entry not matched. Abort matching if we can't ignore this entry. */
            if (input_times[idx] > MAX_IGNORED_BLIP_TIME)
                break;
        }

        if (event_timer > 0)
            event_timer--;
        else
            event_count = 0;

        if (output_timer > 0)
            output_timer--;
        else
            GPIOD->ODR |= (1<<4);

        /* Sample touch sensor inputs */
        inputs = ((GPIOD->IDR >> 5) & 3);
        if (inputs != input_log[log_idx]) {
            log_idx = (log_idx + 1) % LOG_LEN;
            input_log[log_idx] = inputs;
            input_times[log_idx] = 1;
        } else {
            input_times[log_idx]++;
        }
    }
}

/* from https://github.com/Hoksmur/stm8_routines */
static inline void delay_cycles(uint16_t cycles) {
    __asm__("nop\n nop\n"); 
    do {          // ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
        cycles--; //      2c;                 1c;     2c    ; 1/2c   
    } while ( cycles );
    __asm__("nop\n");
}

void _delay_us(uint16_t us) { 
    delay_cycles(((F_CPU * us / 1000000UL)-5)/5);
}

void _delay_ms(uint16_t ms) { 
    while (ms--)
        _delay_us(1000);
}

void assert_failed(uint8_t* file, uint32_t line) {
    UNUSED(file); UNUSED(line);
    while (1);
}
