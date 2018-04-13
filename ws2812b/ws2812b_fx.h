#ifndef __WS2812B_FX
#define __WS2812B_FX

#include "ws2812b_LLD.h"
#include "stm8s.h"

void Wheel(uint16_t i, unsigned char WheelPos);
void rainbowCycle(uint8_t wait);
#endif
