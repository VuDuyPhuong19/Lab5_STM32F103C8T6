#include "PLL.h"
#include <stdint.h>
#include "stm32f10x.h"

void PLL_Init(void){
	// Enable HSE(high-speed external) clock
	RCC->CR |= RCC_CR_HSEON; // RCC_CR_HSEON = 0x00010000
	while(!(RCC->CR & RCC_CR_HSERDY)); // RCC_CR_HSERDY = 0x0002000
	
	// Configuration PLL
	RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE | RCC_CFGR_PLLMULL9; //Set PLL multiplication factor to 9 (72MHz)
	
	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	
	// Set PLL as system clock source and configure the prescalers
	RCC->CFGR &= ~(RCC_CFGR_SW | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
	RCC->CFGR |= (RCC_CFGR_SW_PLL | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1);
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
