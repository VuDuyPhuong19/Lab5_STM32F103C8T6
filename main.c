#include <stdint.h>
#include "SysTick.h"
#include "PLL.h"
#include "stm32f10x.h"

struct State {
	uint32_t out_traffic;
	uint32_t out_walk;
	uint32_t time;
	uint32_t next[8];
};
typedef const struct State State_t;

#define goS 0
#define waitS 1
#define allStop1 2
#define goW 3
#define waitW 4 
#define allStop2 5
#define walk 6
#define onred1 7
#define offred1 8
#define onred2 9
#define offred2 10
#define onred3 11
#define offred3 12
#define allStop3 13
State_t FSM[14] = {
	{0x101, 0x2000, 200, {goS,waitS,goS,waitS,waitS,waitS,waitS,waitS}},   										 					// goS
	{0x102, 0x2000, 100, {allStop1,allStop1,allStop1,allStop1,allStop1,allStop1,allStop1,allStop1}},			// waitS		
	{0x120, 0x2000, 50, {goW,goW,goS,goW,walk,goW,walk,goW}},																						// allStop1
	{0x060, 0x2000, 200, {goW,goW,waitW,waitW,waitW,waitW,waitW,waitW}},												 				// goW	
	{0x0A0, 0x2000, 100, {allStop2,allStop2,allStop2,allStop2,allStop2,allStop2,allStop2,allStop2}},			// waitW
	{0x120, 0x2000, 50, {goS,goW,goS,walk,walk,walk,walk,walk}},
	{0x120, 0x4000, 200, {walk,onred1,onred1,onred1,walk,onred1,onred1,onred1}},
	{0x120, 0x2000, 40, {offred1,offred1,offred1,offred1,offred1,offred1,offred1,offred1}},
	{0x120, 0x0000, 40, {onred2,onred2,onred2,onred2,onred2,onred2,onred2,onred2}},
	{0x120, 0x2000, 40, {offred2,offred2,offred2,offred2,offred2,offred2,offred2,offred2}},
	{0x120, 0x0000, 40, {onred3,onred3,onred3,onred3,onred3,onred3,onred3,onred3}},
	{0x120, 0x2000, 40, {offred3,offred3,offred3,offred3,offred3,offred3,offred3,offred3}},
	{0x120, 0x0000, 40, {allStop3,allStop3,allStop3,allStop3,allStop3,allStop3,allStop3,allStop3}},
	{0x120, 0x2000, 50, {goS,goW,goS,goS,walk,goW,goS,goS}}																			 					// allStop2
};

// switches A0(west),A1(south),A2(walk)
// walklight C13(red),C14(green)
// trafficlight B0(green_south),B1(yellow_south),B5(red_south),B6(green_west),B7(yellow_west),B8(red_west)
void GPIO_Config(void){
	// Initialize GPIO ports 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN; // PortA,PortB,PortC
	
	// set A0,A1,A2 as switches
	GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0 | GPIO_CRL_MODE1 | GPIO_CRL_CNF1 | GPIO_CRL_MODE2 |GPIO_CRL_CNF2);
	GPIOA->CRL |= GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1; // pull up mode
	
	// set B3,B4,B5,B6,B7,B8 as trafic light output
	GPIOB->CRL &= ~(0xFFF000FF);
	GPIOB->CRH &= ~(0x0000000F);
	GPIOB->CRL |= GPIO_CRL_MODE0_0 | GPIO_CRL_MODE1_0 | GPIO_CRL_MODE5_0 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0;
	GPIOB->CRH |= GPIO_CRH_MODE8_0;
	
	// set C13,C14 as walk light output
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13 | GPIO_CRH_MODE14 | GPIO_CRH_CNF14);
	GPIOC->CRH |= GPIO_CRH_MODE13_0 | GPIO_CRH_MODE14_0;
}
int main(void){
	PLL_Init();
	SysTick_Init();
	
	GPIO_Config();
	
	// Initialize state
	uint32_t S;
	uint32_t Input;
	S = goS;
	
	while(1){
		// set traffic light
		GPIOB->ODR = (GPIOB->ODR & ~(0x1E3)) | FSM[S].out_traffic;
		 
		// set walk light
		GPIOC->ODR = (GPIOC->ODR & ~(0x6000)) | FSM[S].out_walk;
		
		// wait 
		SysTick_Wait10ms(FSM[S].time);
	
		// read input
		Input = GPIOA->IDR & 0x07;
		
		// next state depends on input and current state
		S = FSM[S].next[Input];
		
}
}	