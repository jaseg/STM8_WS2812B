#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "stm8s_conf.h"
#include "stm8s_tim2.h"

#define UNUSED(x) ((void)(x))

void _delay_ms(uint16_t ms);
void _delay_us(uint16_t us);

void main(void) {  
    CLK->CKDIVR = 0x00;
    CLK->PCKENR1 |= 0x20;

    GPIOD->DDR |= (1<<1) | (1<<2) | (1<<3);

    TIM2->CR1   =  0x01;
    TIM2->CCMR3 =  0x60;
    TIM2->CCER2 =  0x01;
    TIM2->ARRH  =  0x01;
    TIM2->ARRL  =  0x00;

    while(1) { 
        _delay_ms(50);
        /* run every 50ms */
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
