#ifndef __systick_h_
#define __systick_h_
#include <stdint.h>

// Initialize SysTick with busy wait ruuning at bus clock
void SysTick_Init(void);

// Time delay using busy wait
// The delay parameter is in units of the core clock
void SysTick_Wait(uint32_t delay);

// Time delay using busy wait
// This assumes 72MHz system clock
void SysTick_Wait10ms(uint32_t delay);

#endif
