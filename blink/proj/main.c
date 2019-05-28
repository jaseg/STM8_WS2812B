#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "stm8s_conf.h"
#include "ws2812b_fx.h"
#include "stm8s_tim2.h"

#define D_PINS ((1<<1) | (1<<2) | (1<<3)| (1<<4)| (1<<5) | (1<<6))
#define B_PINS ((1<<4) | (1<<5))
#define C_PINS ((1<<3) | (1<<4) | (1<<5)| (1<<6)| (1<<7))

unsigned char LedsArray[NB_LEDS * 3];
unsigned int nbLedsBytes = NB_LEDS * 3;

unsigned char random();
uint8_t flicker(uint8_t ch);
void _delay_ms(uint16_t ms);
void _delay_us(uint16_t us);

void set_stars(uint16_t mask) {
    GPIOD->ODR &= ~(D_PINS);
    GPIOB->ODR &= ~(B_PINS);
    GPIOC->ODR &= ~(C_PINS);

    mask = ~mask;
    if (mask & 0x01)
        GPIOD->ODR |= 1<<1;
    if (mask & 0x02)
        GPIOD->ODR |= 1<<2;
    if (mask & 0x04)
        GPIOD->ODR |= 1<<3;
    if (mask & 0x08)
        GPIOD->ODR |= 1<<4;
    if (mask & 0x10)
        GPIOD->ODR |= 1<<5;
    if (mask & 0x20)
        GPIOD->ODR |= 1<<6;
    if (mask & 0x40)
        GPIOB->ODR |= 1<<4;
    if (mask & 0x80)
        GPIOB->ODR |= 1<<5;
    if (mask & 0x100)
        GPIOC->ODR |= 1<<3;
    if (mask & 0x200)
        GPIOC->ODR |= 1<<4;
    if (mask & 0x400)
        GPIOC->ODR |= 1<<5;
    if (mask & 0x800)
        GPIOC->ODR |= 1<<6;
    if (mask & 0x1000)
        GPIOC->ODR |= 1<<7;
}

void main(void) {  
    unsigned int i, j, k, l, m;
    CLK->CKDIVR = 0x00;
    CLK->PCKENR1 |= 0x20;

    GPIOD->DDR |= D_PINS;
    GPIOB->DDR |= B_PINS;
    GPIOC->DDR |= C_PINS;
    GPIOA->DDR |= (1<<3);
    GPIOA->CR1 |=  0x08;
    GPIOA->CR2 &= ~0x08;

    TIM2->CR1   =  0x01;
    TIM2->CCMR3 =  0x60;
    TIM2->CCER2 =  0x01;
    TIM2->ARRH  =  0x01;
    TIM2->ARRL  =  0x00;

    //enableInterrupts();

    i = 0;
    j = 0;
    k = 0;
    l = 100;
    m = 0;
    while(1) { 
        //TIM2->CCR3L = 0;
        //_delay_us(5000);
        //_delay_ms(25);

        _delay_ms(50);
        /* run every 50ms */

        if (k == 0)
            set_stars(0);
        else if (k == 3)
            set_stars(1<<(random() % 13));

        if (!m) {
            l += 1;
            if (l > 250) {
                l = 250;
                m = !m;
            }
        } else {
            l -= 1;
            if (l < 100) {
                l = 100;
                m = !m;
            }
        }
        TIM2->CCR3L = l;

        k++;
        if (k == 4)
            k = 0;
    }
}

/* from https://github.com/EternityForest/CandleFlickerSimulator/blob/master/flicker2.X/main.c */
#define WIND_VARIABILITY 5
#define FLAME_AGILITY 2
#define WIND_CALMNESS_CONSTANT 2
#define WIND_BASELINE 30

#define NGROUPS 1
unsigned char flame[NGROUPS], flameprime[NGROUPS], wind[NGROUPS], x, a, b, c;

unsigned char random() {
    x++; //x is incremented every round and is not affected by any other variable
    a = (a ^ c ^ x); //note the mix of addition and XOR
    b = (b + a); //And the use of very few instructions
    c = ((c + (b >> 1) ^ a)); // the AES S-Box Operation ensures an even distributon of entropy
    return (c);
}

uint8_t flicker(uint8_t ch) {
    //We simulate a gust of wind by setting the wind var to a random value
    if (random() < WIND_VARIABILITY)
        //Make a gust of wind less likely with two random teata because 255 is not enough resolution
        if(random() > 220)
             wind[ch] = random();

    //The wind constantly settles towards its baseline value
    if (wind[ch]>WIND_BASELINE)
        wind[ch]--;

    //The flame constantly gets brighter till the wind knocks it down
    if (flame[ch]<255)
        flame[ch]++;

    //Depending on the wind strength and the calmnes modifer we calcuate the odds
    //of the wind knocking down the flame by setting it to random values
    if (random() < (wind[ch]>>WIND_CALMNESS_CONSTANT))
        flame[ch] = random();

    //Real flames ook like they have inertia so we use this constant-aproach-rate filter
    //To lowpass the flame height
    if (flame[ch] > flameprime[ch]) {
        if (flameprime[ch] < (255 - FLAME_AGILITY))
            flameprime[ch] += FLAME_AGILITY;
    } else if (flameprime[ch] > (FLAME_AGILITY))
            flameprime[ch] -= FLAME_AGILITY;
    //How do we prevent jittering when the two are equal?
    //We don't. It adds to the realism.
    return flameprime[ch];
}

void uptime_routine(void) {
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
    while (1);
}
