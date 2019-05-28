                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 3.7.0 #10231 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _uptime_routine
                                     12 	.globl _main
                                     13 	.globl _set_stars
                                     14 	.globl _nbLedsBytes
                                     15 	.globl _c
                                     16 	.globl _b
                                     17 	.globl _a
                                     18 	.globl _x
                                     19 	.globl _wind
                                     20 	.globl _flameprime
                                     21 	.globl _flame
                                     22 	.globl _LedsArray
                                     23 	.globl _random
                                     24 	.globl _flicker
                                     25 	.globl __delay_us
                                     26 	.globl __delay_ms
                                     27 	.globl _assert_failed
                                     28 ;--------------------------------------------------------
                                     29 ; ram data
                                     30 ;--------------------------------------------------------
                                     31 	.area DATA
      000001                         32 _LedsArray::
      000001                         33 	.ds 150
      000097                         34 _flame::
      000097                         35 	.ds 1
      000098                         36 _flameprime::
      000098                         37 	.ds 1
      000099                         38 _wind::
      000099                         39 	.ds 1
      00009A                         40 _x::
      00009A                         41 	.ds 1
      00009B                         42 _a::
      00009B                         43 	.ds 1
      00009C                         44 _b::
      00009C                         45 	.ds 1
      00009D                         46 _c::
      00009D                         47 	.ds 1
                                     48 ;--------------------------------------------------------
                                     49 ; ram data
                                     50 ;--------------------------------------------------------
                                     51 	.area INITIALIZED
      00009E                         52 _nbLedsBytes::
      00009E                         53 	.ds 2
                                     54 ;--------------------------------------------------------
                                     55 ; Stack segment in internal ram 
                                     56 ;--------------------------------------------------------
                                     57 	.area	SSEG
      FFFFFF                         58 __start__stack:
      FFFFFF                         59 	.ds	1
                                     60 
                                     61 ;--------------------------------------------------------
                                     62 ; absolute external ram data
                                     63 ;--------------------------------------------------------
                                     64 	.area DABS (ABS)
                                     65 ;--------------------------------------------------------
                                     66 ; interrupt vector 
                                     67 ;--------------------------------------------------------
                                     68 	.area HOME
      008000                         69 __interrupt_vect:
      008000 82 00 80 07             70 	int s_GSINIT ; reset
                                     71 ;--------------------------------------------------------
                                     72 ; global & static initialisations
                                     73 ;--------------------------------------------------------
                                     74 	.area HOME
                                     75 	.area GSINIT
                                     76 	.area GSFINAL
                                     77 	.area GSINIT
      008007                         78 __sdcc_gs_init_startup:
      008007                         79 __sdcc_init_data:
                                     80 ; stm8_genXINIT() start
      008007 AE 00 9D         [ 2]   81 	ldw x, #l_DATA
      00800A 27 07            [ 1]   82 	jreq	00002$
      00800C                         83 00001$:
      00800C 72 4F 00 00      [ 1]   84 	clr (s_DATA - 1, x)
      008010 5A               [ 2]   85 	decw x
      008011 26 F9            [ 1]   86 	jrne	00001$
      008013                         87 00002$:
      008013 AE 00 02         [ 2]   88 	ldw	x, #l_INITIALIZER
      008016 27 09            [ 1]   89 	jreq	00004$
      008018                         90 00003$:
      008018 D6 86 56         [ 1]   91 	ld	a, (s_INITIALIZER - 1, x)
      00801B D7 00 9D         [ 1]   92 	ld	(s_INITIALIZED - 1, x), a
      00801E 5A               [ 2]   93 	decw	x
      00801F 26 F7            [ 1]   94 	jrne	00003$
      008021                         95 00004$:
                                     96 ; stm8_genXINIT() end
                                     97 	.area GSFINAL
      008021 CC 80 04         [ 2]   98 	jp	__sdcc_program_startup
                                     99 ;--------------------------------------------------------
                                    100 ; Home
                                    101 ;--------------------------------------------------------
                                    102 	.area HOME
                                    103 	.area HOME
      008004                        104 __sdcc_program_startup:
      008004 CC 80 C1         [ 2]  105 	jp	_main
                                    106 ;	return from main will return to caller
                                    107 ;--------------------------------------------------------
                                    108 ; code
                                    109 ;--------------------------------------------------------
                                    110 	.area CODE
                                    111 ;	proj/main.c: 20: void set_stars(uint16_t mask) {
                                    112 ;	-----------------------------------------
                                    113 ;	 function set_stars
                                    114 ;	-----------------------------------------
      008024                        115 _set_stars:
                                    116 ;	proj/main.c: 21: GPIOD->ODR &= ~(D_PINS);
      008024 C6 50 0F         [ 1]  117 	ld	a, 0x500f
      008027 A4 81            [ 1]  118 	and	a, #0x81
      008029 C7 50 0F         [ 1]  119 	ld	0x500f, a
                                    120 ;	proj/main.c: 22: GPIOB->ODR &= ~(B_PINS);
      00802C C6 50 05         [ 1]  121 	ld	a, 0x5005
      00802F A4 CF            [ 1]  122 	and	a, #0xcf
      008031 C7 50 05         [ 1]  123 	ld	0x5005, a
                                    124 ;	proj/main.c: 23: GPIOC->ODR &= ~(C_PINS);
      008034 C6 50 0A         [ 1]  125 	ld	a, 0x500a
      008037 A4 07            [ 1]  126 	and	a, #0x07
      008039 C7 50 0A         [ 1]  127 	ld	0x500a, a
                                    128 ;	proj/main.c: 25: mask = ~mask;
      00803C 1E 03            [ 2]  129 	ldw	x, (0x03, sp)
      00803E 53               [ 2]  130 	cplw	x
      00803F 1F 03            [ 2]  131 	ldw	(0x03, sp), x
                                    132 ;	proj/main.c: 26: if (mask & 0x01)
      008041 7B 04            [ 1]  133 	ld	a, (0x04, sp)
      008043 44               [ 1]  134 	srl	a
      008044 24 04            [ 1]  135 	jrnc	00102$
                                    136 ;	proj/main.c: 27: GPIOD->ODR |= 1<<1;
      008046 72 12 50 0F      [ 1]  137 	bset	20495, #1
      00804A                        138 00102$:
                                    139 ;	proj/main.c: 28: if (mask & 0x02)
      00804A 7B 04            [ 1]  140 	ld	a, (0x04, sp)
      00804C A5 02            [ 1]  141 	bcp	a, #0x02
      00804E 27 04            [ 1]  142 	jreq	00104$
                                    143 ;	proj/main.c: 29: GPIOD->ODR |= 1<<2;
      008050 72 14 50 0F      [ 1]  144 	bset	20495, #2
      008054                        145 00104$:
                                    146 ;	proj/main.c: 30: if (mask & 0x04)
      008054 7B 04            [ 1]  147 	ld	a, (0x04, sp)
      008056 A5 04            [ 1]  148 	bcp	a, #0x04
      008058 27 04            [ 1]  149 	jreq	00106$
                                    150 ;	proj/main.c: 31: GPIOD->ODR |= 1<<3;
      00805A 72 16 50 0F      [ 1]  151 	bset	20495, #3
      00805E                        152 00106$:
                                    153 ;	proj/main.c: 32: if (mask & 0x08)
      00805E 7B 04            [ 1]  154 	ld	a, (0x04, sp)
      008060 A5 08            [ 1]  155 	bcp	a, #0x08
      008062 27 04            [ 1]  156 	jreq	00108$
                                    157 ;	proj/main.c: 33: GPIOD->ODR |= 1<<4;
      008064 72 18 50 0F      [ 1]  158 	bset	20495, #4
      008068                        159 00108$:
                                    160 ;	proj/main.c: 34: if (mask & 0x10)
      008068 7B 04            [ 1]  161 	ld	a, (0x04, sp)
      00806A A5 10            [ 1]  162 	bcp	a, #0x10
      00806C 27 04            [ 1]  163 	jreq	00110$
                                    164 ;	proj/main.c: 35: GPIOD->ODR |= 1<<5;
      00806E 72 1A 50 0F      [ 1]  165 	bset	20495, #5
      008072                        166 00110$:
                                    167 ;	proj/main.c: 36: if (mask & 0x20)
      008072 7B 04            [ 1]  168 	ld	a, (0x04, sp)
      008074 A5 20            [ 1]  169 	bcp	a, #0x20
      008076 27 04            [ 1]  170 	jreq	00112$
                                    171 ;	proj/main.c: 37: GPIOD->ODR |= 1<<6;
      008078 72 1C 50 0F      [ 1]  172 	bset	20495, #6
      00807C                        173 00112$:
                                    174 ;	proj/main.c: 38: if (mask & 0x40)
      00807C 7B 04            [ 1]  175 	ld	a, (0x04, sp)
      00807E A5 40            [ 1]  176 	bcp	a, #0x40
      008080 27 04            [ 1]  177 	jreq	00114$
                                    178 ;	proj/main.c: 39: GPIOB->ODR |= 1<<4;
      008082 72 18 50 05      [ 1]  179 	bset	20485, #4
      008086                        180 00114$:
                                    181 ;	proj/main.c: 40: if (mask & 0x80)
      008086 0D 04            [ 1]  182 	tnz	(0x04, sp)
      008088 2A 04            [ 1]  183 	jrpl	00116$
                                    184 ;	proj/main.c: 41: GPIOB->ODR |= 1<<5;
      00808A 72 1A 50 05      [ 1]  185 	bset	20485, #5
      00808E                        186 00116$:
                                    187 ;	proj/main.c: 42: if (mask & 0x100)
      00808E 7B 03            [ 1]  188 	ld	a, (0x03, sp)
      008090 44               [ 1]  189 	srl	a
      008091 24 04            [ 1]  190 	jrnc	00118$
                                    191 ;	proj/main.c: 43: GPIOC->ODR |= 1<<3;
      008093 72 16 50 0A      [ 1]  192 	bset	20490, #3
      008097                        193 00118$:
                                    194 ;	proj/main.c: 44: if (mask & 0x200)
      008097 7B 03            [ 1]  195 	ld	a, (0x03, sp)
      008099 A5 02            [ 1]  196 	bcp	a, #0x02
      00809B 27 04            [ 1]  197 	jreq	00120$
                                    198 ;	proj/main.c: 45: GPIOC->ODR |= 1<<4;
      00809D 72 18 50 0A      [ 1]  199 	bset	20490, #4
      0080A1                        200 00120$:
                                    201 ;	proj/main.c: 46: if (mask & 0x400)
      0080A1 7B 03            [ 1]  202 	ld	a, (0x03, sp)
      0080A3 A5 04            [ 1]  203 	bcp	a, #0x04
      0080A5 27 04            [ 1]  204 	jreq	00122$
                                    205 ;	proj/main.c: 47: GPIOC->ODR |= 1<<5;
      0080A7 72 1A 50 0A      [ 1]  206 	bset	20490, #5
      0080AB                        207 00122$:
                                    208 ;	proj/main.c: 48: if (mask & 0x800)
      0080AB 7B 03            [ 1]  209 	ld	a, (0x03, sp)
      0080AD A5 08            [ 1]  210 	bcp	a, #0x08
      0080AF 27 04            [ 1]  211 	jreq	00124$
                                    212 ;	proj/main.c: 49: GPIOC->ODR |= 1<<6;
      0080B1 72 1C 50 0A      [ 1]  213 	bset	20490, #6
      0080B5                        214 00124$:
                                    215 ;	proj/main.c: 50: if (mask & 0x1000)
      0080B5 7B 03            [ 1]  216 	ld	a, (0x03, sp)
      0080B7 A5 10            [ 1]  217 	bcp	a, #0x10
      0080B9 26 01            [ 1]  218 	jrne	00193$
      0080BB 81               [ 4]  219 	ret
      0080BC                        220 00193$:
                                    221 ;	proj/main.c: 51: GPIOC->ODR |= 1<<7;
      0080BC 72 1E 50 0A      [ 1]  222 	bset	20490, #7
                                    223 ;	proj/main.c: 52: }
      0080C0 81               [ 4]  224 	ret
                                    225 ;	proj/main.c: 54: void main(void) {  
                                    226 ;	-----------------------------------------
                                    227 ;	 function main
                                    228 ;	-----------------------------------------
      0080C1                        229 _main:
      0080C1 52 08            [ 2]  230 	sub	sp, #8
                                    231 ;	proj/main.c: 56: CLK->CKDIVR = 0x00;
      0080C3 35 00 50 C6      [ 1]  232 	mov	0x50c6+0, #0x00
                                    233 ;	proj/main.c: 57: CLK->PCKENR1 |= 0x20;
      0080C7 C6 50 C7         [ 1]  234 	ld	a, 0x50c7
      0080CA 41               [ 1]  235 	exg	a, xl
      0080CB 4F               [ 1]  236 	clr	a
      0080CC 41               [ 1]  237 	exg	a, xl
      0080CD AA 20            [ 1]  238 	or	a, #0x20
      0080CF 88               [ 1]  239 	push	a
      0080D0 84               [ 1]  240 	pop	a
      0080D1 C7 50 C7         [ 1]  241 	ld	0x50c7, a
                                    242 ;	proj/main.c: 59: GPIOD->DDR |= D_PINS;
      0080D4 C6 50 11         [ 1]  243 	ld	a, 0x5011
      0080D7 AA 7E            [ 1]  244 	or	a, #0x7e
      0080D9 C7 50 11         [ 1]  245 	ld	0x5011, a
                                    246 ;	proj/main.c: 60: GPIOB->DDR |= B_PINS;
      0080DC C6 50 07         [ 1]  247 	ld	a, 0x5007
      0080DF AA 30            [ 1]  248 	or	a, #0x30
      0080E1 C7 50 07         [ 1]  249 	ld	0x5007, a
                                    250 ;	proj/main.c: 61: GPIOC->DDR |= C_PINS;
      0080E4 C6 50 0C         [ 1]  251 	ld	a, 0x500c
      0080E7 AA F8            [ 1]  252 	or	a, #0xf8
      0080E9 C7 50 0C         [ 1]  253 	ld	0x500c, a
                                    254 ;	proj/main.c: 62: GPIOA->DDR |= (1<<3);
      0080EC 72 16 50 02      [ 1]  255 	bset	20482, #3
                                    256 ;	proj/main.c: 63: GPIOA->CR1 |=  0x08;
      0080F0 C6 50 03         [ 1]  257 	ld	a, 0x5003
      0080F3 41               [ 1]  258 	exg	a, xl
      0080F4 4F               [ 1]  259 	clr	a
      0080F5 41               [ 1]  260 	exg	a, xl
      0080F6 AA 08            [ 1]  261 	or	a, #0x08
      0080F8 88               [ 1]  262 	push	a
      0080F9 84               [ 1]  263 	pop	a
      0080FA C7 50 03         [ 1]  264 	ld	0x5003, a
                                    265 ;	proj/main.c: 64: GPIOA->CR2 &= ~0x08;
      0080FD C6 50 04         [ 1]  266 	ld	a, 0x5004
      008100 A4 F7            [ 1]  267 	and	a, #0xf7
      008102 C7 50 04         [ 1]  268 	ld	0x5004, a
                                    269 ;	proj/main.c: 66: TIM2->CR1   =  0x01;
      008105 35 01 53 00      [ 1]  270 	mov	0x5300+0, #0x01
                                    271 ;	proj/main.c: 67: TIM2->CCMR3 =  0x60;
      008109 35 60 53 09      [ 1]  272 	mov	0x5309+0, #0x60
                                    273 ;	proj/main.c: 68: TIM2->CCER2 =  0x01;
      00810D 35 01 53 0B      [ 1]  274 	mov	0x530b+0, #0x01
                                    275 ;	proj/main.c: 69: TIM2->ARRH  =  0x01;
      008111 35 01 53 0F      [ 1]  276 	mov	0x530f+0, #0x01
                                    277 ;	proj/main.c: 70: TIM2->ARRL  =  0x00;
      008115 35 00 53 10      [ 1]  278 	mov	0x5310+0, #0x00
                                    279 ;	proj/main.c: 76: k = 0;
      008119 5F               [ 1]  280 	clrw	x
      00811A 1F 07            [ 2]  281 	ldw	(0x07, sp), x
                                    282 ;	proj/main.c: 77: l = 100;
      00811C AE 00 64         [ 2]  283 	ldw	x, #0x0064
      00811F 1F 05            [ 2]  284 	ldw	(0x05, sp), x
                                    285 ;	proj/main.c: 78: m = 0;
      008121 5F               [ 1]  286 	clrw	x
      008122 1F 03            [ 2]  287 	ldw	(0x03, sp), x
                                    288 ;	proj/main.c: 79: while(1) { 
      008124                        289 00116$:
                                    290 ;	proj/main.c: 84: _delay_ms(50);
      008124 4B 32            [ 1]  291 	push	#0x32
      008126 4B 00            [ 1]  292 	push	#0x00
      008128 CD 82 B9         [ 4]  293 	call	__delay_ms
      00812B 5B 02            [ 2]  294 	addw	sp, #2
                                    295 ;	proj/main.c: 87: if (k == 0)
      00812D 1E 07            [ 2]  296 	ldw	x, (0x07, sp)
      00812F 26 09            [ 1]  297 	jrne	00104$
                                    298 ;	proj/main.c: 88: set_stars(0);
      008131 5F               [ 1]  299 	clrw	x
      008132 89               [ 2]  300 	pushw	x
      008133 CD 80 24         [ 4]  301 	call	_set_stars
      008136 5B 02            [ 2]  302 	addw	sp, #2
      008138 20 26            [ 2]  303 	jra	00105$
      00813A                        304 00104$:
                                    305 ;	proj/main.c: 89: else if (k == 3)
      00813A 1E 07            [ 2]  306 	ldw	x, (0x07, sp)
      00813C A3 00 03         [ 2]  307 	cpw	x, #0x0003
      00813F 26 1F            [ 1]  308 	jrne	00105$
                                    309 ;	proj/main.c: 90: set_stars(1<<(random() % 13));
      008141 CD 81 B7         [ 4]  310 	call	_random
      008144 5F               [ 1]  311 	clrw	x
      008145 97               [ 1]  312 	ld	xl, a
      008146 4B 0D            [ 1]  313 	push	#0x0d
      008148 4B 00            [ 1]  314 	push	#0x00
      00814A 89               [ 2]  315 	pushw	x
      00814B CD 85 C5         [ 4]  316 	call	__modsint
      00814E 5B 04            [ 2]  317 	addw	sp, #4
      008150 9F               [ 1]  318 	ld	a, xl
      008151 5F               [ 1]  319 	clrw	x
      008152 5C               [ 1]  320 	incw	x
      008153 4D               [ 1]  321 	tnz	a
      008154 27 04            [ 1]  322 	jreq	00153$
      008156                        323 00152$:
      008156 58               [ 2]  324 	sllw	x
      008157 4A               [ 1]  325 	dec	a
      008158 26 FC            [ 1]  326 	jrne	00152$
      00815A                        327 00153$:
      00815A 89               [ 2]  328 	pushw	x
      00815B CD 80 24         [ 4]  329 	call	_set_stars
      00815E 5B 02            [ 2]  330 	addw	sp, #2
      008160                        331 00105$:
                                    332 ;	proj/main.c: 96: m = !m;
      008160 7B 04            [ 1]  333 	ld	a, (0x04, sp)
      008162 A0 01            [ 1]  334 	sub	a, #0x01
      008164 7B 03            [ 1]  335 	ld	a, (0x03, sp)
      008166 A2 00            [ 1]  336 	sbc	a, #0x00
      008168 4F               [ 1]  337 	clr	a
      008169 49               [ 1]  338 	rlc	a
      00816A 0F 01            [ 1]  339 	clr	(0x01, sp)
                                    340 ;	proj/main.c: 92: if (!m) {
      00816C 1E 03            [ 2]  341 	ldw	x, (0x03, sp)
      00816E 26 17            [ 1]  342 	jrne	00111$
                                    343 ;	proj/main.c: 93: l += 1;
      008170 1E 05            [ 2]  344 	ldw	x, (0x05, sp)
      008172 5C               [ 1]  345 	incw	x
                                    346 ;	proj/main.c: 94: if (l > 250) {
      008173 1F 05            [ 2]  347 	ldw	(0x05, sp), x
      008175 A3 00 FA         [ 2]  348 	cpw	x, #0x00fa
      008178 23 22            [ 2]  349 	jrule	00112$
                                    350 ;	proj/main.c: 95: l = 250;
      00817A AE 00 FA         [ 2]  351 	ldw	x, #0x00fa
      00817D 1F 05            [ 2]  352 	ldw	(0x05, sp), x
                                    353 ;	proj/main.c: 96: m = !m;
      00817F 6B 04            [ 1]  354 	ld	(0x04, sp), a
      008181 7B 01            [ 1]  355 	ld	a, (0x01, sp)
      008183 6B 03            [ 1]  356 	ld	(0x03, sp), a
      008185 20 15            [ 2]  357 	jra	00112$
      008187                        358 00111$:
                                    359 ;	proj/main.c: 99: l -= 1;
      008187 1E 05            [ 2]  360 	ldw	x, (0x05, sp)
      008189 5A               [ 2]  361 	decw	x
                                    362 ;	proj/main.c: 100: if (l < 100) {
      00818A 1F 05            [ 2]  363 	ldw	(0x05, sp), x
      00818C A3 00 64         [ 2]  364 	cpw	x, #0x0064
      00818F 24 0B            [ 1]  365 	jrnc	00112$
                                    366 ;	proj/main.c: 101: l = 100;
      008191 AE 00 64         [ 2]  367 	ldw	x, #0x0064
      008194 1F 05            [ 2]  368 	ldw	(0x05, sp), x
                                    369 ;	proj/main.c: 102: m = !m;
      008196 6B 04            [ 1]  370 	ld	(0x04, sp), a
      008198 7B 01            [ 1]  371 	ld	a, (0x01, sp)
      00819A 6B 03            [ 1]  372 	ld	(0x03, sp), a
      00819C                        373 00112$:
                                    374 ;	proj/main.c: 105: TIM2->CCR3L = l;
      00819C 7B 06            [ 1]  375 	ld	a, (0x06, sp)
      00819E C7 53 16         [ 1]  376 	ld	0x5316, a
                                    377 ;	proj/main.c: 107: k++;
      0081A1 1E 07            [ 2]  378 	ldw	x, (0x07, sp)
      0081A3 5C               [ 1]  379 	incw	x
                                    380 ;	proj/main.c: 108: if (k == 4)
      0081A4 1F 07            [ 2]  381 	ldw	(0x07, sp), x
      0081A6 A3 00 04         [ 2]  382 	cpw	x, #0x0004
      0081A9 27 03            [ 1]  383 	jreq	00159$
      0081AB CC 81 24         [ 2]  384 	jp	00116$
      0081AE                        385 00159$:
                                    386 ;	proj/main.c: 109: k = 0;
      0081AE 5F               [ 1]  387 	clrw	x
      0081AF 1F 07            [ 2]  388 	ldw	(0x07, sp), x
      0081B1 CC 81 24         [ 2]  389 	jp	00116$
                                    390 ;	proj/main.c: 111: }
      0081B4 5B 08            [ 2]  391 	addw	sp, #8
      0081B6 81               [ 4]  392 	ret
                                    393 ;	proj/main.c: 122: unsigned char random() {
                                    394 ;	-----------------------------------------
                                    395 ;	 function random
                                    396 ;	-----------------------------------------
      0081B7                        397 _random:
                                    398 ;	proj/main.c: 123: x++; //x is incremented every round and is not affected by any other variable
      0081B7 72 5C 00 9A      [ 1]  399 	inc	_x+0
                                    400 ;	proj/main.c: 124: a = (a ^ c ^ x); //note the mix of addition and XOR
      0081BB C6 00 9D         [ 1]  401 	ld	a, _c+0
      0081BE C8 00 9B         [ 1]  402 	xor	a, _a+0
      0081C1 C8 00 9A         [ 1]  403 	xor	a, _x+0
      0081C4 C7 00 9B         [ 1]  404 	ld	_a+0, a
                                    405 ;	proj/main.c: 125: b = (b + a); //And the use of very few instructions
      0081C7 C6 00 9C         [ 1]  406 	ld	a, _b+0
      0081CA CB 00 9B         [ 1]  407 	add	a, _a+0
                                    408 ;	proj/main.c: 126: c = ((c + (b >> 1) ^ a)); // the AES S-Box Operation ensures an even distributon of entropy
      0081CD C7 00 9C         [ 1]  409 	ld	_b+0, a
      0081D0 44               [ 1]  410 	srl	a
      0081D1 CB 00 9D         [ 1]  411 	add	a, _c+0
      0081D4 C8 00 9B         [ 1]  412 	xor	a, _a+0
                                    413 ;	proj/main.c: 127: return (c);
      0081D7 C7 00 9D         [ 1]  414 	ld	_c+0, a
                                    415 ;	proj/main.c: 128: }
      0081DA 81               [ 4]  416 	ret
                                    417 ;	proj/main.c: 130: uint8_t flicker(uint8_t ch) {
                                    418 ;	-----------------------------------------
                                    419 ;	 function flicker
                                    420 ;	-----------------------------------------
      0081DB                        421 _flicker:
      0081DB 52 03            [ 2]  422 	sub	sp, #3
                                    423 ;	proj/main.c: 132: if (random() < WIND_VARIABILITY)
      0081DD CD 81 B7         [ 4]  424 	call	_random
      0081E0 A1 05            [ 1]  425 	cp	a, #0x05
      0081E2 24 17            [ 1]  426 	jrnc	00104$
                                    427 ;	proj/main.c: 134: if(random() > 220)
      0081E4 CD 81 B7         [ 4]  428 	call	_random
      0081E7 A1 DC            [ 1]  429 	cp	a, #0xdc
      0081E9 23 10            [ 2]  430 	jrule	00104$
                                    431 ;	proj/main.c: 135: wind[ch] = random();
      0081EB AE 00 99         [ 2]  432 	ldw	x, #_wind+0
      0081EE 9F               [ 1]  433 	ld	a, xl
      0081EF 1B 06            [ 1]  434 	add	a, (0x06, sp)
      0081F1 02               [ 1]  435 	rlwa	x
      0081F2 A9 00            [ 1]  436 	adc	a, #0x00
      0081F4 95               [ 1]  437 	ld	xh, a
      0081F5 89               [ 2]  438 	pushw	x
      0081F6 CD 81 B7         [ 4]  439 	call	_random
      0081F9 85               [ 2]  440 	popw	x
      0081FA F7               [ 1]  441 	ld	(x), a
      0081FB                        442 00104$:
                                    443 ;	proj/main.c: 138: if (wind[ch]>WIND_BASELINE)
      0081FB AE 00 99         [ 2]  444 	ldw	x, #_wind+0
      0081FE 9F               [ 1]  445 	ld	a, xl
      0081FF 1B 06            [ 1]  446 	add	a, (0x06, sp)
      008201 90 97            [ 1]  447 	ld	yl, a
      008203 9E               [ 1]  448 	ld	a, xh
      008204 A9 00            [ 1]  449 	adc	a, #0x00
      008206 90 95            [ 1]  450 	ld	yh, a
      008208 90 F6            [ 1]  451 	ld	a, (y)
      00820A A1 1E            [ 1]  452 	cp	a, #0x1e
      00820C 23 03            [ 2]  453 	jrule	00106$
                                    454 ;	proj/main.c: 139: wind[ch]--;
      00820E 4A               [ 1]  455 	dec	a
      00820F 90 F7            [ 1]  456 	ld	(y), a
      008211                        457 00106$:
                                    458 ;	proj/main.c: 142: if (flame[ch]<255)
      008211 AE 00 97         [ 2]  459 	ldw	x, #_flame+0
      008214 9F               [ 1]  460 	ld	a, xl
      008215 1B 06            [ 1]  461 	add	a, (0x06, sp)
      008217 02               [ 1]  462 	rlwa	x
      008218 A9 00            [ 1]  463 	adc	a, #0x00
      00821A 95               [ 1]  464 	ld	xh, a
      00821B F6               [ 1]  465 	ld	a, (x)
      00821C A1 FF            [ 1]  466 	cp	a, #0xff
      00821E 24 02            [ 1]  467 	jrnc	00108$
                                    468 ;	proj/main.c: 143: flame[ch]++;
      008220 4C               [ 1]  469 	inc	a
      008221 F7               [ 1]  470 	ld	(x), a
      008222                        471 00108$:
                                    472 ;	proj/main.c: 147: if (random() < (wind[ch]>>WIND_CALMNESS_CONSTANT))
      008222 89               [ 2]  473 	pushw	x
      008223 90 89            [ 2]  474 	pushw	y
      008225 CD 81 B7         [ 4]  475 	call	_random
      008228 6B 07            [ 1]  476 	ld	(0x07, sp), a
      00822A 90 85            [ 2]  477 	popw	y
      00822C 85               [ 2]  478 	popw	x
      00822D 90 F6            [ 1]  479 	ld	a, (y)
      00822F 44               [ 1]  480 	srl	a
      008230 44               [ 1]  481 	srl	a
      008231 11 03            [ 1]  482 	cp	a, (0x03, sp)
      008233 23 06            [ 2]  483 	jrule	00110$
                                    484 ;	proj/main.c: 148: flame[ch] = random();
      008235 89               [ 2]  485 	pushw	x
      008236 CD 81 B7         [ 4]  486 	call	_random
      008239 85               [ 2]  487 	popw	x
      00823A F7               [ 1]  488 	ld	(x), a
      00823B                        489 00110$:
                                    490 ;	proj/main.c: 152: if (flame[ch] > flameprime[ch]) {
      00823B F6               [ 1]  491 	ld	a, (x)
      00823C 6B 02            [ 1]  492 	ld	(0x02, sp), a
      00823E AE 00 98         [ 2]  493 	ldw	x, #_flameprime+0
      008241 9F               [ 1]  494 	ld	a, xl
      008242 1B 06            [ 1]  495 	add	a, (0x06, sp)
      008244 02               [ 1]  496 	rlwa	x
      008245 A9 00            [ 1]  497 	adc	a, #0x00
      008247 95               [ 1]  498 	ld	xh, a
      008248 F6               [ 1]  499 	ld	a, (x)
                                    500 ;	proj/main.c: 154: flameprime[ch] += FLAME_AGILITY;
      008249 6B 01            [ 1]  501 	ld	(0x01, sp), a
                                    502 ;	proj/main.c: 152: if (flame[ch] > flameprime[ch]) {
      00824B 11 02            [ 1]  503 	cp	a, (0x02, sp)
      00824D 24 0B            [ 1]  504 	jrnc	00116$
                                    505 ;	proj/main.c: 153: if (flameprime[ch] < (255 - FLAME_AGILITY))
      00824F A1 FD            [ 1]  506 	cp	a, #0xfd
      008251 24 10            [ 1]  507 	jrnc	00117$
                                    508 ;	proj/main.c: 154: flameprime[ch] += FLAME_AGILITY;
      008253 7B 01            [ 1]  509 	ld	a, (0x01, sp)
      008255 AB 02            [ 1]  510 	add	a, #0x02
      008257 F7               [ 1]  511 	ld	(x), a
      008258 20 09            [ 2]  512 	jra	00117$
      00825A                        513 00116$:
                                    514 ;	proj/main.c: 155: } else if (flameprime[ch] > (FLAME_AGILITY))
      00825A A1 02            [ 1]  515 	cp	a, #0x02
      00825C 23 05            [ 2]  516 	jrule	00117$
                                    517 ;	proj/main.c: 156: flameprime[ch] -= FLAME_AGILITY;
      00825E 7B 01            [ 1]  518 	ld	a, (0x01, sp)
      008260 A0 02            [ 1]  519 	sub	a, #0x02
      008262 F7               [ 1]  520 	ld	(x), a
      008263                        521 00117$:
                                    522 ;	proj/main.c: 159: return flameprime[ch];
      008263 F6               [ 1]  523 	ld	a, (x)
                                    524 ;	proj/main.c: 160: }
      008264 5B 03            [ 2]  525 	addw	sp, #3
      008266 81               [ 4]  526 	ret
                                    527 ;	proj/main.c: 162: void uptime_routine(void) {
                                    528 ;	-----------------------------------------
                                    529 ;	 function uptime_routine
                                    530 ;	-----------------------------------------
      008267                        531 _uptime_routine:
                                    532 ;	proj/main.c: 163: }
      008267 81               [ 4]  533 	ret
                                    534 ;	proj/main.c: 166: static inline void delay_cycles(uint16_t cycles) {
                                    535 ;	-----------------------------------------
                                    536 ;	 function delay_cycles
                                    537 ;	-----------------------------------------
      008268                        538 _delay_cycles:
                                    539 ;	proj/main.c: 167: __asm__("nop\n nop\n"); 
      008268 9D               [ 1]  540 	nop
      008269 9D               [ 1]  541 	nop
                                    542 ;	proj/main.c: 168: do {          // ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
      00826A 1E 03            [ 2]  543 	ldw	x, (0x03, sp)
      00826C                        544 00101$:
                                    545 ;	proj/main.c: 169: cycles--; //      2c;                 1c;     2c    ; 1/2c   
      00826C 5A               [ 2]  546 	decw	x
                                    547 ;	proj/main.c: 170: } while ( cycles );
      00826D 5D               [ 2]  548 	tnzw	x
      00826E 26 FC            [ 1]  549 	jrne	00101$
                                    550 ;	proj/main.c: 171: __asm__("nop\n");
      008270 9D               [ 1]  551 	nop
                                    552 ;	proj/main.c: 172: }
      008271 81               [ 4]  553 	ret
                                    554 ;	proj/main.c: 174: void _delay_us(uint16_t us) { 
                                    555 ;	-----------------------------------------
                                    556 ;	 function _delay_us
                                    557 ;	-----------------------------------------
      008272                        558 __delay_us:
                                    559 ;	proj/main.c: 175: delay_cycles(((F_CPU * us / 1000000UL)-5)/5);
      008272 16 03            [ 2]  560 	ldw	y, (0x03, sp)
      008274 5F               [ 1]  561 	clrw	x
      008275 90 89            [ 2]  562 	pushw	y
      008277 89               [ 2]  563 	pushw	x
      008278 4B 00            [ 1]  564 	push	#0x00
      00827A 4B 24            [ 1]  565 	push	#0x24
      00827C 4B F4            [ 1]  566 	push	#0xf4
      00827E 4B 00            [ 1]  567 	push	#0x00
      008280 CD 85 DB         [ 4]  568 	call	__mullong
      008283 5B 08            [ 2]  569 	addw	sp, #8
      008285 4B 40            [ 1]  570 	push	#0x40
      008287 4B 42            [ 1]  571 	push	#0x42
      008289 4B 0F            [ 1]  572 	push	#0x0f
      00828B 4B 00            [ 1]  573 	push	#0x00
      00828D 89               [ 2]  574 	pushw	x
      00828E 90 89            [ 2]  575 	pushw	y
      008290 CD 85 6B         [ 4]  576 	call	__divulong
      008293 5B 08            [ 2]  577 	addw	sp, #8
      008295 51               [ 1]  578 	exgw	x, y
      008296 72 A2 00 05      [ 2]  579 	subw	y, #0x0005
      00829A 9F               [ 1]  580 	ld	a, xl
      00829B A2 00            [ 1]  581 	sbc	a, #0x00
      00829D 02               [ 1]  582 	rlwa	x
      00829E A2 00            [ 1]  583 	sbc	a, #0x00
      0082A0 95               [ 1]  584 	ld	xh, a
      0082A1 4B 05            [ 1]  585 	push	#0x05
      0082A3 4B 00            [ 1]  586 	push	#0x00
      0082A5 4B 00            [ 1]  587 	push	#0x00
      0082A7 4B 00            [ 1]  588 	push	#0x00
      0082A9 90 89            [ 2]  589 	pushw	y
      0082AB 89               [ 2]  590 	pushw	x
      0082AC CD 85 6B         [ 4]  591 	call	__divulong
      0082AF 5B 08            [ 2]  592 	addw	sp, #8
                                    593 ;	proj/main.c: 167: __asm__("nop\n nop\n"); 
      0082B1 9D               [ 1]  594 	nop
      0082B2 9D               [ 1]  595 	nop
                                    596 ;	proj/main.c: 168: do {          // ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
      0082B3                        597 00101$:
                                    598 ;	proj/main.c: 169: cycles--; //      2c;                 1c;     2c    ; 1/2c   
      0082B3 5A               [ 2]  599 	decw	x
                                    600 ;	proj/main.c: 170: } while ( cycles );
      0082B4 5D               [ 2]  601 	tnzw	x
      0082B5 26 FC            [ 1]  602 	jrne	00101$
                                    603 ;	proj/main.c: 171: __asm__("nop\n");
      0082B7 9D               [ 1]  604 	nop
                                    605 ;	proj/main.c: 175: delay_cycles(((F_CPU * us / 1000000UL)-5)/5);
                                    606 ;	proj/main.c: 176: }
      0082B8 81               [ 4]  607 	ret
                                    608 ;	proj/main.c: 178: void _delay_ms(uint16_t ms) { 
                                    609 ;	-----------------------------------------
                                    610 ;	 function _delay_ms
                                    611 ;	-----------------------------------------
      0082B9                        612 __delay_ms:
                                    613 ;	proj/main.c: 179: while (ms--)
      0082B9 1E 03            [ 2]  614 	ldw	x, (0x03, sp)
      0082BB                        615 00101$:
      0082BB 90 93            [ 1]  616 	ldw	y, x
      0082BD 5A               [ 2]  617 	decw	x
      0082BE 90 5D            [ 2]  618 	tnzw	y
      0082C0 26 01            [ 1]  619 	jrne	00115$
      0082C2 81               [ 4]  620 	ret
      0082C3                        621 00115$:
                                    622 ;	proj/main.c: 180: _delay_us(1000);
      0082C3 89               [ 2]  623 	pushw	x
      0082C4 4B E8            [ 1]  624 	push	#0xe8
      0082C6 4B 03            [ 1]  625 	push	#0x03
      0082C8 CD 82 72         [ 4]  626 	call	__delay_us
      0082CB 5B 02            [ 2]  627 	addw	sp, #2
      0082CD 85               [ 2]  628 	popw	x
      0082CE 20 EB            [ 2]  629 	jra	00101$
                                    630 ;	proj/main.c: 181: }
      0082D0 81               [ 4]  631 	ret
                                    632 ;	proj/main.c: 183: void assert_failed(uint8_t* file, uint32_t line) {
                                    633 ;	-----------------------------------------
                                    634 ;	 function assert_failed
                                    635 ;	-----------------------------------------
      0082D1                        636 _assert_failed:
                                    637 ;	proj/main.c: 184: while (1);
      0082D1                        638 00102$:
      0082D1 20 FE            [ 2]  639 	jra	00102$
                                    640 ;	proj/main.c: 185: }
      0082D3 81               [ 4]  641 	ret
                                    642 	.area CODE
                                    643 	.area INITIALIZER
      008657                        644 __xinit__nbLedsBytes:
      008657 00 96                  645 	.dw #0x0096
                                    646 	.area CABS (ABS)
