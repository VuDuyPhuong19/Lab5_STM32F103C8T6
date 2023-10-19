#include <stdint.h>
#include "stm32f10x.h"
#include "SysTick.h"

// Initialize SysTick with bus wait at bus clock
void SysTick_Init(void){
	SysTick->CTRL = 0;
	SysTick->LOAD = SysTick_LOAD_RELOAD;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE | SysTick_CTRL_CLKSOURCE;
}

void SysTick_Wait(uint32_t delay){ // wait 13.8ns
	SysTick->LOAD = delay - 1;
	SysTick->VAL = 0;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0) {}
}

void SysTick_Wait10ms(uint32_t delay){
	uint32_t i;
	for(i = 0; i < delay; i++){
		SysTick_Wait(720000); // wait 10ms
	}
}