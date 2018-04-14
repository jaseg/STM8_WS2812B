#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "stm8s_conf.h"
#include "ws2812b_fx.h"
#include "stm8s_tim2.h"

#define GROUP_SIZE 4
#define NGROUPS (sizeof(LedsArray)/sizeof(LedsArray[0])/GROUP_SIZE/3)

unsigned char LedsArray[NB_LEDS * 3];
unsigned int nbLedsBytes = NB_LEDS * 3;

uint8_t flicker(uint8_t ch);
void _delay_ms(uint16_t ms);
void _delay_us(uint16_t us);


void main(void) {  
    uint16_t i;
    uint8_t j, x, y, z;
    uint8_t inval[8];

    CLK_DeInit();
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK->PCKENR1 |= 0x20;

    GPIO_Init (GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init (GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);

    GPIOA->DDR |=  0x08;
    GPIOA->CR1 |=  0x08;
    GPIOA->CR2 &= ~0x08;

    GPIOC->DDR &= ~0xf0;
    GPIOC->CR1 |=  0xf0;
    GPIOC->CR2 &= ~0xf0;

    TIM2->CR1   =  0x01;
    TIM2->CCMR3 =  0x60;
    TIM2->CCER2 =  0x01;
    TIM2->ARRH  =  0x01;
    TIM2->ARRL  =  0x00;

    TIM2->CCR3L =  0x80;
    //TIM2->EGR   =  0x01;

    //enableInterrupts();

    while(1) { 
        /* x: cold white
         * y: amber
         * z: warm white
         */

        x = 0;
        j = 0;
        TIM2->CCR3L = 0;
        do {
            _delay_us(10);
            y = GPIOC->IDR>>4;
            TIM2->CCR3L = j+1;

            z = ~x & y;
            if (z&1) inval[0] = j;
            if (z&2) inval[1] = j;
            if (z&4) inval[2] = j;
            if (z&8) inval[3] = j;
            j++;
        } while(j);

        for (i=0; i<sizeof(LedsArray)/sizeof(LedsArray[0])/GROUP_SIZE/3; i++) {
            //x = y = z = 0;
            //y = z = flicker(i);
            //z >>= 1; /* adjust color temperature: use amber at full power, but
            //            warm white only at half output. Amber is pretty dim. */
            x = y = z = 0;
            //x = GPIOC->IDR & ((1<<4)<<(i&3)) ? 16 : 0;
            x = inval[i&3];
            if (x < 32)
                x = 0;
            else if (x >= 224)
                x = 255;
            else {
                x -= 32;
                x = x + (x>>2);
            }

            y = x;
            x = x>64 ? x-64 : 0;
            z = ((x>>4) * (x>>4));
            x = 0;

            for (j=0; j<GROUP_SIZE; j++) {
                LedsArray[i*3*GROUP_SIZE+j*3+0] = x; //(i&1 ? j : ~j)&7;
                LedsArray[i*3*GROUP_SIZE+j*3+1] = y; //(i&2 ? j : ~j)&7;
                LedsArray[i*3*GROUP_SIZE+j*3+2] = z; //(i&4 ? j : ~j)&7;
            }
        }
        rgb_SendArray();
        //j = ~j;
        //_delay_ms(25);
    }
}

/* from https://github.com/EternityForest/CandleFlickerSimulator/blob/master/flicker2.X/main.c */
#define WIND_VARIABILITY 5
#define FLAME_AGILITY 2
#define WIND_CALMNESS_CONSTANT 2
#define WIND_BASELINE 30

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
