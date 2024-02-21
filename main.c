#include <stdint.h>
#include "msp.h"
void TA0_N_IRQHandler(void);
void T32_INT1_IRQHandler(void);
void PORT1_IRQHandler(void);
static uint8_t interruptSource = 0x00;
int main(void)
{
	WDT_A->CTL = WDT_A_CTL_HOLD | WDT_A_CTL_PW;
	//red led, rgb-red led, button 1&2
	P1->SEL0 &= ~(BIT0|BIT1|BIT4);
	P1->SEL1 &= ~(BIT0|BIT1|BIT4);
	P1->DIR  |= BIT0;
	P1->DS   &= ~BIT0;
	P1->OUT  &= ~BIT0;
  P2->SEL0 &= ~BIT0;
	P2->SEL1 &= ~BIT0;
	P2->DIR  |= BIT0;
	P2->DS   &= ~BIT0;
	P2->OUT  &= ~BIT0;
	P1->DIR  &= ~(BIT1|BIT4);
	P1->REN  |= BIT1|BIT4;
	P1->OUT  |= BIT1|BIT4;
	P1->IES  |= BIT1 ;
	P1->IES  |= BIT4 ;
	P1->IFG  &= ~BIT1;
	P1->IFG  &= ~BIT4;
	P1->IE   |= BIT1;
	P1->IE   |= BIT4;
	
	//xxxx xx01 xx01 xx10  01- ACLK = 32768Hz. 01-up mode. 1-interrupt enable. 0-cleared ifg
	TIMER_A0->CTL |= BIT8;
	TIMER_A0->CTL |= BIT1;
	TIMER_A0->CTL &= ~(BIT5 | BIT4);
	TIMER_A0->CTL &= ~BIT0;
	TIMER_A0->CCR[0] = 0x7FFF; //32767

	NVIC_ClearPendingIRQ(TA0_N_IRQn);
	NVIC_SetPriority(TA0_N_IRQn, 2);
	NVIC_EnableIRQ(TA0_N_IRQn);
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	NVIC_SetPriority(PORT1_IRQn, 2);
	NVIC_EnableIRQ(PORT1_IRQn);
	
  
	//0 = WRAPPING MODE. 1 = ONE-SHOT 
	TIMER32_1->CONTROL |= BIT0;
	//1 = 32-BIT COUNTER. 0 = 16-BIT COUNTER
	TIMER32_1->CONTROL |=  BIT1;
	//PRESCALE/CLOCK DIVIDER. 00b = / BY 1. 01b = / BY 16. 10b = / BY 256
	TIMER32_1->CONTROL |=  BIT2;
	TIMER32_1->CONTROL &= (uint8_t)~BIT3;
	//1 = TIMER32 INTERRUPT ENABLED. 0 = DISABLED
	TIMER32_1->CONTROL |=  BIT5;
	//0 = FREE RUNNING MODE. 1 = PERIODIC MODE
	TIMER32_1->CONTROL |=  BIT6;
	//1 = TIMER32 ENABLED. 0 = TIMER32 DISABLED
	TIMER32_1->CONTROL &= (uint8_t) ~BIT7;
	//COUNT DOWN FROM LOAD VALUE. 3MHZ / 16 = 187500 OR 1 SECOND. 675,000,000 is 1hour
	TIMER32_1->LOAD = 675000000;
	//CLEAR INTERRUPT
	TIMER32_1->INTCLR = 0x0;
	//NVIC CONFIG
	NVIC_ClearPendingIRQ(T32_INT1_IRQn);
	NVIC_SetPriority(T32_INT1_IRQn, 2);
	NVIC_EnableIRQ(T32_INT1_IRQn);
	//ENABLE INTERRUPTS
	__enable_irq();

	
	while(1)
	{
		__WFI();
	}
	

}
void PORT1_IRQHandler(void){
	interruptSource = P1->IFG;
	P1->IFG &= ~BIT1;
  P1->IFG &= ~BIT4;
	if((interruptSource & BIT1) != 0){
		TIMER32_1->CONTROL |= (uint8_t) BIT7;
		TIMER32_1->LOAD = 675000000;
		TIMER32_1->INTCLR = 0x0;
		P1->OUT  &= (uint8_t)~BIT0;
	}
	else if((interruptSource& BIT4) != 0)
	{
		TIMER_A0->CTL |= BIT4;
		P2->OUT  &= (uint8_t)~BIT0; 
	}

}
void T32_INT1_IRQHandler(void){
	TIMER32_1->INTCLR = 0x0;
	P1->OUT  |= (uint8_t)BIT0;
	
}
void TA0_N_IRQHandler(void)
{
	TIMER_A0->CTL &= ~BIT0;
	static uint8_t counter = 0;
	++counter;
	if(counter == 240){
		P2->OUT  |= (uint8_t)BIT0; 
		counter = 0;
		TIMER_A0->CTL &= ~BIT4;
	}
	
}


