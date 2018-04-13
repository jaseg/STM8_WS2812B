#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "stm8s_conf.h"
#include "ws2812b_fx.h"

#define GROUP_SIZE 4
#define NGROUPS (sizeof(LedsArray)/sizeof(LedsArray[0])/GROUP_SIZE/3)

unsigned char LedsArray[NB_LEDS * 3];
unsigned int nbLedsBytes = NB_LEDS * 3;

uint8_t flicker(uint8_t ch);
void _delay_ms(uint16_t wait);


void main(void) {  
    uint16_t i;
    uint8_t j, x, y, z;

    CLK_DeInit();
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    GPIO_Init (GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init (GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);

    enableInterrupts();

    while(1) { 
        /* x: cold white
         * y: amber
         * z: warm white
         */
        for (i=0; i<sizeof(LedsArray)/sizeof(LedsArray[0])/GROUP_SIZE/3; i++) {
            x = y = z = 0;
            y = z = flicker(i);
            z >>= 1; /* adjust color temperature: use amber at full power, but
                        warm white only at half output. Amber is pretty dim. */
            for (j=0; j<GROUP_SIZE; j++) {
                LedsArray[i*3*GROUP_SIZE+j*3+0] = x; //(i&1 ? j : ~j)&7;
                LedsArray[i*3*GROUP_SIZE+j*3+1] = y; //(i&2 ? j : ~j)&7;
                LedsArray[i*3*GROUP_SIZE+j*3+2] = z; //(i&4 ? j : ~j)&7;
            }
        }
        rgb_SendArray();
        //j = ~j;
        _delay_ms(25);
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
