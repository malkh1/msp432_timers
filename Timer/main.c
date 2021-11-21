#include <stdint.h>
#include "msp.h"
void TA0_N_IRQHandler(void);
void T32_INT1_IRQHandler(void);
void PORT1_IRQHandler(void);
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
	
	//xxxx xx01 0010 0010
	//01-32768hz clk. 00-divby1. 01-continuous mode.00- do it. 1 int enable. 0 clear ifg
	//TIMER_A0->CTL = 0x0122; //-> 0000 0001 0010 0010

	//LIMIT*/
	//TIMER_A0->CCR[0] = 0xFFFF;
	NVIC_ClearPendingIRQ(TA0_N_IRQn);
	NVIC_SetPriority(TA0_N_IRQn, 2);
	NVIC_EnableIRQ(TA0_N_IRQn);
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	NVIC_SetPriority(PORT1_IRQn, 2);
	NVIC_EnableIRQ(PORT1_IRQn);
	
  
	//0 = WRAPPING MODE. 1 = ONE-SHOT 
	TIMER32_1->CONTROL |= (uint8_t)BIT0;
	//1 = 32-BIT COUNTER. 0 = 16-BIT COUNTER
	TIMER32_1->CONTROL |= (uint8_t) BIT1;
	//PRESCALE/CLOCK DIVIDER. 00b = / BY 1. 01b = / BY 16. 10b = / BY 256
	TIMER32_1->CONTROL |= (uint8_t) BIT2;
	TIMER32_1->CONTROL &= (uint8_t)~BIT3;
	//1 = TIMER32 INTERRUPT ENABLED. 0 = DISABLED
	TIMER32_1->CONTROL |= (uint8_t) BIT5;
	//0 = FREE RUNNING MODE. 1 = PERIODIC MODE
	TIMER32_1->CONTROL |= (uint8_t) BIT6;
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
	if((P1->IFG & BIT1) != 0){
		TIMER32_1->CONTROL |= (uint8_t) BIT7;
		TIMER32_1->LOAD = 675000000;
		TIMER32_1->INTCLR = 0x0;
		P1->OUT  &= (uint8_t)~BIT0;
	}
	else if((P1->IFG & BIT4) != 0)
	{
		TIMER_A0->CTL = 0x0122;
		P2->OUT  &= (uint8_t)~BIT0; 
	}
		P1->IFG &= ~BIT1;
	  P1->IFG &= ~BIT4;
}
void T32_INT1_IRQHandler(void){
	TIMER32_1->INTCLR = 0x0;
	P1->OUT  |= (uint8_t)BIT0;
	
}
void TA0_N_IRQHandler(void)
{
	static uint8_t counter = 0;
	++counter;
	if(counter == 240){
		P2->OUT  |= (uint8_t)BIT0; 
		counter = 0;
		TIMER_A0->CTL = 0x0000;
	}
	TIMER_A0->CTL &= ~BIT0;
}


