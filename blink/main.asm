;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.0 #10231 (Linux)
;--------------------------------------------------------
	.module main
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _uptime_routine
	.globl _main
	.globl _set_stars
	.globl _nbLedsBytes
	.globl _c
	.globl _b
	.globl _a
	.globl _x
	.globl _wind
	.globl _flameprime
	.globl _flame
	.globl _LedsArray
	.globl _random
	.globl _flicker
	.globl __delay_us
	.globl __delay_ms
	.globl _assert_failed
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
_LedsArray::
	.ds 150
_flame::
	.ds 1
_flameprime::
	.ds 1
_wind::
	.ds 1
_x::
	.ds 1
_a::
	.ds 1
_b::
	.ds 1
_c::
	.ds 1
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
_nbLedsBytes::
	.ds 2
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
__sdcc_gs_init_startup:
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	proj/main.c: 20: void set_stars(uint16_t mask) {
;	-----------------------------------------
;	 function set_stars
;	-----------------------------------------
_set_stars:
;	proj/main.c: 21: GPIOD->ODR &= ~(D_PINS);
	ld	a, 0x500f
	and	a, #0x81
	ld	0x500f, a
;	proj/main.c: 22: GPIOB->ODR &= ~(B_PINS);
	ld	a, 0x5005
	and	a, #0xcf
	ld	0x5005, a
;	proj/main.c: 23: GPIOC->ODR &= ~(C_PINS);
	ld	a, 0x500a
	and	a, #0x07
	ld	0x500a, a
;	proj/main.c: 25: mask = ~mask;
	ldw	x, (0x03, sp)
	cplw	x
	ldw	(0x03, sp), x
;	proj/main.c: 26: if (mask & 0x01)
	ld	a, (0x04, sp)
	srl	a
	jrnc	00102$
;	proj/main.c: 27: GPIOD->ODR |= 1<<1;
	bset	20495, #1
00102$:
;	proj/main.c: 28: if (mask & 0x02)
	ld	a, (0x04, sp)
	bcp	a, #0x02
	jreq	00104$
;	proj/main.c: 29: GPIOD->ODR |= 1<<2;
	bset	20495, #2
00104$:
;	proj/main.c: 30: if (mask & 0x04)
	ld	a, (0x04, sp)
	bcp	a, #0x04
	jreq	00106$
;	proj/main.c: 31: GPIOD->ODR |= 1<<3;
	bset	20495, #3
00106$:
;	proj/main.c: 32: if (mask & 0x08)
	ld	a, (0x04, sp)
	bcp	a, #0x08
	jreq	00108$
;	proj/main.c: 33: GPIOD->ODR |= 1<<4;
	bset	20495, #4
00108$:
;	proj/main.c: 34: if (mask & 0x10)
	ld	a, (0x04, sp)
	bcp	a, #0x10
	jreq	00110$
;	proj/main.c: 35: GPIOD->ODR |= 1<<5;
	bset	20495, #5
00110$:
;	proj/main.c: 36: if (mask & 0x20)
	ld	a, (0x04, sp)
	bcp	a, #0x20
	jreq	00112$
;	proj/main.c: 37: GPIOD->ODR |= 1<<6;
	bset	20495, #6
00112$:
;	proj/main.c: 38: if (mask & 0x40)
	ld	a, (0x04, sp)
	bcp	a, #0x40
	jreq	00114$
;	proj/main.c: 39: GPIOB->ODR |= 1<<4;
	bset	20485, #4
00114$:
;	proj/main.c: 40: if (mask & 0x80)
	tnz	(0x04, sp)
	jrpl	00116$
;	proj/main.c: 41: GPIOB->ODR |= 1<<5;
	bset	20485, #5
00116$:
;	proj/main.c: 42: if (mask & 0x100)
	ld	a, (0x03, sp)
	srl	a
	jrnc	00118$
;	proj/main.c: 43: GPIOC->ODR |= 1<<3;
	bset	20490, #3
00118$:
;	proj/main.c: 44: if (mask & 0x200)
	ld	a, (0x03, sp)
	bcp	a, #0x02
	jreq	00120$
;	proj/main.c: 45: GPIOC->ODR |= 1<<4;
	bset	20490, #4
00120$:
;	proj/main.c: 46: if (mask & 0x400)
	ld	a, (0x03, sp)
	bcp	a, #0x04
	jreq	00122$
;	proj/main.c: 47: GPIOC->ODR |= 1<<5;
	bset	20490, #5
00122$:
;	proj/main.c: 48: if (mask & 0x800)
	ld	a, (0x03, sp)
	bcp	a, #0x08
	jreq	00124$
;	proj/main.c: 49: GPIOC->ODR |= 1<<6;
	bset	20490, #6
00124$:
;	proj/main.c: 50: if (mask & 0x1000)
	ld	a, (0x03, sp)
	bcp	a, #0x10
	jrne	00193$
	ret
00193$:
;	proj/main.c: 51: GPIOC->ODR |= 1<<7;
	bset	20490, #7
;	proj/main.c: 52: }
	ret
;	proj/main.c: 54: void main(void) {  
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	sub	sp, #8
;	proj/main.c: 56: CLK->CKDIVR = 0x00;
	mov	0x50c6+0, #0x00
;	proj/main.c: 57: CLK->PCKENR1 |= 0x20;
	ld	a, 0x50c7
	exg	a, xl
	clr	a
	exg	a, xl
	or	a, #0x20
	push	a
	pop	a
	ld	0x50c7, a
;	proj/main.c: 59: GPIOD->DDR |= D_PINS;
	ld	a, 0x5011
	or	a, #0x7e
	ld	0x5011, a
;	proj/main.c: 60: GPIOB->DDR |= B_PINS;
	ld	a, 0x5007
	or	a, #0x30
	ld	0x5007, a
;	proj/main.c: 61: GPIOC->DDR |= C_PINS;
	ld	a, 0x500c
	or	a, #0xf8
	ld	0x500c, a
;	proj/main.c: 62: GPIOA->DDR |= (1<<3);
	bset	20482, #3
;	proj/main.c: 63: GPIOA->CR1 |=  0x08;
	ld	a, 0x5003
	exg	a, xl
	clr	a
	exg	a, xl
	or	a, #0x08
	push	a
	pop	a
	ld	0x5003, a
;	proj/main.c: 64: GPIOA->CR2 &= ~0x08;
	ld	a, 0x5004
	and	a, #0xf7
	ld	0x5004, a
;	proj/main.c: 66: TIM2->CR1   =  0x01;
	mov	0x5300+0, #0x01
;	proj/main.c: 67: TIM2->CCMR3 =  0x60;
	mov	0x5309+0, #0x60
;	proj/main.c: 68: TIM2->CCER2 =  0x01;
	mov	0x530b+0, #0x01
;	proj/main.c: 69: TIM2->ARRH  =  0x01;
	mov	0x530f+0, #0x01
;	proj/main.c: 70: TIM2->ARRL  =  0x00;
	mov	0x5310+0, #0x00
;	proj/main.c: 76: k = 0;
	clrw	x
	ldw	(0x07, sp), x
;	proj/main.c: 77: l = 100;
	ldw	x, #0x0064
	ldw	(0x05, sp), x
;	proj/main.c: 78: m = 0;
	clrw	x
	ldw	(0x03, sp), x
;	proj/main.c: 79: while(1) { 
00116$:
;	proj/main.c: 84: _delay_ms(50);
	push	#0x32
	push	#0x00
	call	__delay_ms
	addw	sp, #2
;	proj/main.c: 87: if (k == 0)
	ldw	x, (0x07, sp)
	jrne	00104$
;	proj/main.c: 88: set_stars(0);
	clrw	x
	pushw	x
	call	_set_stars
	addw	sp, #2
	jra	00105$
00104$:
;	proj/main.c: 89: else if (k == 3)
	ldw	x, (0x07, sp)
	cpw	x, #0x0003
	jrne	00105$
;	proj/main.c: 90: set_stars(1<<(random() % 13));
	call	_random
	clrw	x
	ld	xl, a
	push	#0x0d
	push	#0x00
	pushw	x
	call	__modsint
	addw	sp, #4
	ld	a, xl
	clrw	x
	incw	x
	tnz	a
	jreq	00153$
00152$:
	sllw	x
	dec	a
	jrne	00152$
00153$:
	pushw	x
	call	_set_stars
	addw	sp, #2
00105$:
;	proj/main.c: 96: m = !m;
	ld	a, (0x04, sp)
	sub	a, #0x01
	ld	a, (0x03, sp)
	sbc	a, #0x00
	clr	a
	rlc	a
	clr	(0x01, sp)
;	proj/main.c: 92: if (!m) {
	ldw	x, (0x03, sp)
	jrne	00111$
;	proj/main.c: 93: l += 1;
	ldw	x, (0x05, sp)
	incw	x
;	proj/main.c: 94: if (l > 250) {
	ldw	(0x05, sp), x
	cpw	x, #0x00fa
	jrule	00112$
;	proj/main.c: 95: l = 250;
	ldw	x, #0x00fa
	ldw	(0x05, sp), x
;	proj/main.c: 96: m = !m;
	ld	(0x04, sp), a
	ld	a, (0x01, sp)
	ld	(0x03, sp), a
	jra	00112$
00111$:
;	proj/main.c: 99: l -= 1;
	ldw	x, (0x05, sp)
	decw	x
;	proj/main.c: 100: if (l < 100) {
	ldw	(0x05, sp), x
	cpw	x, #0x0064
	jrnc	00112$
;	proj/main.c: 101: l = 100;
	ldw	x, #0x0064
	ldw	(0x05, sp), x
;	proj/main.c: 102: m = !m;
	ld	(0x04, sp), a
	ld	a, (0x01, sp)
	ld	(0x03, sp), a
00112$:
;	proj/main.c: 105: TIM2->CCR3L = l;
	ld	a, (0x06, sp)
	ld	0x5316, a
;	proj/main.c: 107: k++;
	ldw	x, (0x07, sp)
	incw	x
;	proj/main.c: 108: if (k == 4)
	ldw	(0x07, sp), x
	cpw	x, #0x0004
	jreq	00159$
	jp	00116$
00159$:
;	proj/main.c: 109: k = 0;
	clrw	x
	ldw	(0x07, sp), x
	jp	00116$
;	proj/main.c: 111: }
	addw	sp, #8
	ret
;	proj/main.c: 122: unsigned char random() {
;	-----------------------------------------
;	 function random
;	-----------------------------------------
_random:
;	proj/main.c: 123: x++; //x is incremented every round and is not affected by any other variable
	inc	_x+0
;	proj/main.c: 124: a = (a ^ c ^ x); //note the mix of addition and XOR
	ld	a, _c+0
	xor	a, _a+0
	xor	a, _x+0
	ld	_a+0, a
;	proj/main.c: 125: b = (b + a); //And the use of very few instructions
	ld	a, _b+0
	add	a, _a+0
;	proj/main.c: 126: c = ((c + (b >> 1) ^ a)); // the AES S-Box Operation ensures an even distributon of entropy
	ld	_b+0, a
	srl	a
	add	a, _c+0
	xor	a, _a+0
;	proj/main.c: 127: return (c);
	ld	_c+0, a
;	proj/main.c: 128: }
	ret
;	proj/main.c: 130: uint8_t flicker(uint8_t ch) {
;	-----------------------------------------
;	 function flicker
;	-----------------------------------------
_flicker:
	sub	sp, #3
;	proj/main.c: 132: if (random() < WIND_VARIABILITY)
	call	_random
	cp	a, #0x05
	jrnc	00104$
;	proj/main.c: 134: if(random() > 220)
	call	_random
	cp	a, #0xdc
	jrule	00104$
;	proj/main.c: 135: wind[ch] = random();
	ldw	x, #_wind+0
	ld	a, xl
	add	a, (0x06, sp)
	rlwa	x
	adc	a, #0x00
	ld	xh, a
	pushw	x
	call	_random
	popw	x
	ld	(x), a
00104$:
;	proj/main.c: 138: if (wind[ch]>WIND_BASELINE)
	ldw	x, #_wind+0
	ld	a, xl
	add	a, (0x06, sp)
	ld	yl, a
	ld	a, xh
	adc	a, #0x00
	ld	yh, a
	ld	a, (y)
	cp	a, #0x1e
	jrule	00106$
;	proj/main.c: 139: wind[ch]--;
	dec	a
	ld	(y), a
00106$:
;	proj/main.c: 142: if (flame[ch]<255)
	ldw	x, #_flame+0
	ld	a, xl
	add	a, (0x06, sp)
	rlwa	x
	adc	a, #0x00
	ld	xh, a
	ld	a, (x)
	cp	a, #0xff
	jrnc	00108$
;	proj/main.c: 143: flame[ch]++;
	inc	a
	ld	(x), a
00108$:
;	proj/main.c: 147: if (random() < (wind[ch]>>WIND_CALMNESS_CONSTANT))
	pushw	x
	pushw	y
	call	_random
	ld	(0x07, sp), a
	popw	y
	popw	x
	ld	a, (y)
	srl	a
	srl	a
	cp	a, (0x03, sp)
	jrule	00110$
;	proj/main.c: 148: flame[ch] = random();
	pushw	x
	call	_random
	popw	x
	ld	(x), a
00110$:
;	proj/main.c: 152: if (flame[ch] > flameprime[ch]) {
	ld	a, (x)
	ld	(0x02, sp), a
	ldw	x, #_flameprime+0
	ld	a, xl
	add	a, (0x06, sp)
	rlwa	x
	adc	a, #0x00
	ld	xh, a
	ld	a, (x)
;	proj/main.c: 154: flameprime[ch] += FLAME_AGILITY;
	ld	(0x01, sp), a
;	proj/main.c: 152: if (flame[ch] > flameprime[ch]) {
	cp	a, (0x02, sp)
	jrnc	00116$
;	proj/main.c: 153: if (flameprime[ch] < (255 - FLAME_AGILITY))
	cp	a, #0xfd
	jrnc	00117$
;	proj/main.c: 154: flameprime[ch] += FLAME_AGILITY;
	ld	a, (0x01, sp)
	add	a, #0x02
	ld	(x), a
	jra	00117$
00116$:
;	proj/main.c: 155: } else if (flameprime[ch] > (FLAME_AGILITY))
	cp	a, #0x02
	jrule	00117$
;	proj/main.c: 156: flameprime[ch] -= FLAME_AGILITY;
	ld	a, (0x01, sp)
	sub	a, #0x02
	ld	(x), a
00117$:
;	proj/main.c: 159: return flameprime[ch];
	ld	a, (x)
;	proj/main.c: 160: }
	addw	sp, #3
	ret
;	proj/main.c: 162: void uptime_routine(void) {
;	-----------------------------------------
;	 function uptime_routine
;	-----------------------------------------
_uptime_routine:
;	proj/main.c: 163: }
	ret
;	proj/main.c: 166: static inline void delay_cycles(uint16_t cycles) {
;	-----------------------------------------
;	 function delay_cycles
;	-----------------------------------------
_delay_cycles:
;	proj/main.c: 167: __asm__("nop\n nop\n"); 
	nop
	nop
;	proj/main.c: 168: do {          // ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
	ldw	x, (0x03, sp)
00101$:
;	proj/main.c: 169: cycles--; //      2c;                 1c;     2c    ; 1/2c   
	decw	x
;	proj/main.c: 170: } while ( cycles );
	tnzw	x
	jrne	00101$
;	proj/main.c: 171: __asm__("nop\n");
	nop
;	proj/main.c: 172: }
	ret
;	proj/main.c: 174: void _delay_us(uint16_t us) { 
;	-----------------------------------------
;	 function _delay_us
;	-----------------------------------------
__delay_us:
;	proj/main.c: 175: delay_cycles(((F_CPU * us / 1000000UL)-5)/5);
	ldw	y, (0x03, sp)
	clrw	x
	pushw	y
	pushw	x
	push	#0x00
	push	#0x24
	push	#0xf4
	push	#0x00
	call	__mullong
	addw	sp, #8
	push	#0x40
	push	#0x42
	push	#0x0f
	push	#0x00
	pushw	x
	pushw	y
	call	__divulong
	addw	sp, #8
	exgw	x, y
	subw	y, #0x0005
	ld	a, xl
	sbc	a, #0x00
	rlwa	x
	sbc	a, #0x00
	ld	xh, a
	push	#0x05
	push	#0x00
	push	#0x00
	push	#0x00
	pushw	y
	pushw	x
	call	__divulong
	addw	sp, #8
;	proj/main.c: 167: __asm__("nop\n nop\n"); 
	nop
	nop
;	proj/main.c: 168: do {          // ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
00101$:
;	proj/main.c: 169: cycles--; //      2c;                 1c;     2c    ; 1/2c   
	decw	x
;	proj/main.c: 170: } while ( cycles );
	tnzw	x
	jrne	00101$
;	proj/main.c: 171: __asm__("nop\n");
	nop
;	proj/main.c: 175: delay_cycles(((F_CPU * us / 1000000UL)-5)/5);
;	proj/main.c: 176: }
	ret
;	proj/main.c: 178: void _delay_ms(uint16_t ms) { 
;	-----------------------------------------
;	 function _delay_ms
;	-----------------------------------------
__delay_ms:
;	proj/main.c: 179: while (ms--)
	ldw	x, (0x03, sp)
00101$:
	ldw	y, x
	decw	x
	tnzw	y
	jrne	00115$
	ret
00115$:
;	proj/main.c: 180: _delay_us(1000);
	pushw	x
	push	#0xe8
	push	#0x03
	call	__delay_us
	addw	sp, #2
	popw	x
	jra	00101$
;	proj/main.c: 181: }
	ret
;	proj/main.c: 183: void assert_failed(uint8_t* file, uint32_t line) {
;	-----------------------------------------
;	 function assert_failed
;	-----------------------------------------
_assert_failed:
;	proj/main.c: 184: while (1);
00102$:
	jra	00102$
;	proj/main.c: 185: }
	ret
	.area CODE
	.area INITIALIZER
__xinit__nbLedsBytes:
	.dw #0x0096
	.area CABS (ABS)
