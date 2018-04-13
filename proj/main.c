#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "stm8s_conf.h"
#include "ws2812b_fx.h"

unsigned char LedsArray[NB_LEDS * 3];
unsigned int nbLedsBytes = NB_LEDS * 3;

void clk_config(void);

void delay() {
    uint16_t i=0;
    while (i < 0xFFFF) {
        i++;
    }
}

void _delay_ms(uint16_t wait);

void main(void) {  
    uint16_t i;
    uint8_t j = 0;

    clk_config();
    GPIO_Init (GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init (GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);

    enableInterrupts();

    while(1) { 
        for (i=0; i<sizeof(LedsArray)/sizeof(LedsArray[0])/3; i++) {
            LedsArray[i*3+0] = (i&1 ? j : ~j)&7;
            LedsArray[i*3+1] = (i&2 ? j : ~j)&7;
            LedsArray[i*3+2] = (i&4 ? j : ~j)&7;
        }
        rgb_SendArray();
        //j = ~j;
        _delay_ms(500);
    }
}


void clk_config(void) {
    CLK_DeInit();
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    /*
     * CLK_HSECmd(ENABLE);
       CLK_ClockSwitchConfig(CLK_SWITCHMODE_MANUAL, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
       CLK_ClockSwitchCmd(ENABLE);
    */
}

void uptime_routine(void) {
}

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
