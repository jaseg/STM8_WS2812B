#include "ws2812b_fx.h"

extern unsigned char LedsArray[NB_LEDS * 3];

void _delay_ms(uint16_t wait);

void Wheel(uint16_t i, unsigned char WheelPos) {
    uint8_t r, g, b;
    WheelPos = 255 - WheelPos;

    if(WheelPos < 85) {
        r = 255 - WheelPos * 3;
        g = 0;
        b = WheelPos * 3;

    } else if(WheelPos < 170) {
        WheelPos -= 85;
        r = 0;
        g = WheelPos * 3;
        b = 255 - WheelPos * 3;

    } else {
        WheelPos -= 170;
        r = WheelPos * 3; 
        g = 255 - WheelPos * 3;
        b = 0;
    }

    LedsArray[i*3+0] = r;
    LedsArray[i*3+1] = g;
    LedsArray[i*3+2] = b;
}



void rainbowCycle(uint8_t wait) 
{
    uint16_t i, j;
    for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i < NB_LEDS; i++) {
            Wheel(i, ((i * 256 / 10) + j) & 255);
        }
        rgb_SendArray();
        _delay_ms(wait);
    }
}

