#include "timer.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>



static TIMER timer2;
static TIMER timer3;
static TIMER timer4;


void TIMER2_init(void)
{
    // Enable Timer 2 clock (p. 152 in the datasheet).
    RCC->APB1ENR |= 0x00000001;
    // Set the frequency of Timer 2.
    // Assuming a clock source of 8 MHz (see pp. 127 and 133 in the datasheet),
    // this will set the timer to tick at a rate of 1 Hz.
    TIM2->ARR = 8000000;
    // Enable the Timer 2 interrupt.
    TIM2->DIER = 0x00000001;
    NVIC_EnableIRQ(TIM2_IRQn);
    // Enable Timer 2.
    TIM2->CR1 = 0x00000001;
}


void TIMER3_init(void){
	// Enable Timer 2 clock (p. 152 in the datasheet).
	    RCC->APB1ENR |= 0x00000001;
	    // Set the frequency of Timer 3.
	    // Assuming a clock source of 8 MHz (see pp. 127 and 133 in the datasheet),
	    // this will set the timer to tick at a rate of 1 Hz.
	    TIM3->ARR = 8000000;
	    // Enable the Timer 2 interrupt.
	    TIM3->DIER = 0x00000001;
	    NVIC_EnableIRQ(TIM3_IRQn);
	    // Enable Timer 3.
	    TIM3->CR1 = 0x00000001;

}

void TIMER4_init(void)
{
	// Enable Timer 4 clock (p. 152 in the datasheet).
	    RCC->APB1ENR |= 0x00000001;
	    // Set the frequency of Timer 4.
	    // Assuming a clock source of 8 MHz (see pp. 127 and 133 in the datasheet),
	    // this will set the timer to tick at a rate of 1 Hz.
	    TIM4->ARR = 8000000;
	    // Enable the Timer 2 interrupt.
	    TIM4->DIER = 0x00000001;
	    NVIC_EnableIRQ(TIM4_IRQn);
	    // Enable Timer 2.
	    TIM4->CR1 = 0x00000001;

}


void TIMER2_enable(void)
{
	TIM2->CR1 |= 0x0001; // CEN: Counter enable
}

void TIMER3_enable(void)
{
	TIM3->CR1 |= 0x0001; // CEN: Counter enable
}

void TIMER4_enable(void)
{
	TIM4->CR1 |= 0x0001; // CEN: Counter enable
}


void TIMER2_disable(void)
{
	TIM2->CR1 &= ~(0x0001); // CEN: Counter disable
}

void TIMER3_disable(void)
{
	TIM3->CR1 &= ~(0x0001); // CEN: Counter disable
}

void TIMER4_disable(void)
{
	TIM4->CR1 &= ~(0x0001); // CEN: Counter disable

}


void TIMER2_delay(uint32_t num_of_sec)
{
	TIMER2_enable();

	timer2.countTicks = 0;
	while(timer2.countTicks < num_of_sec);

	TIMER2_disable();

}

void TIMER3_delay(uint32_t num_of_sec)
{
	TIMER3_enable();

	timer3.countTicks = 0;
	while(timer3.countTicks < num_of_sec);
	TIMER3_disable();

}

void TIMER4_delay(uint32_t num_of_sec)
{
	TIMER4_enable();

	timer4.countTicks = 0;
	while(timer4.countTicks < num_of_sec);

	TIMER4_disable();

}


void TIMER2_set_timeout(uint32_t num_of_sec)
{
	timer2.targetTick = num_of_sec;
	timer2.countTicks = 0;
	TIMER2_enable();
}

BOOL TIMER2_timeout_done(void){


	if(timer2.countTicks >= timer2.targetTick){

		TIMER2_disable();
		return TRUE;
	}
	return FALSE;

}


void TIMER3_set_timeout(uint32_t num_of_sec)
{
	timer3.targetTick = num_of_sec;
	timer3.countTicks = 0;
	TIMER3_enable();
}

BOOL TIMER3_timeout_done(void){

	if(timer3.countTicks >= timer3.targetTick){

		TIMER3_disable();
		return TRUE;
	}
	return FALSE;

}


void TIMER4_set_timeout(uint32_t num_of_seconds)
{
	timer4.targetTick = num_of_seconds;
	timer4.countTicks = 0;
	TIMER4_enable();
}

BOOL TIMER4_timeout_done(void){

	if(timer4.countTicks >= timer4.targetTick){

		TIMER4_disable();
		return TRUE;
	}
	return FALSE;

}


void TIM2_IRQHandler(void)
{
	if((TIM2->SR & 0x0001) == 0x0001)
	{
		TIM2->SR &= ~(0x0001);
		timer2.countTicks++;

		// avoid overflow
		if((timer2.countTicks == MAX_COUNT) || (timer2.countTicks < 0)){
			timer2.countTicks = 0;
		}

	}
	// USART2_write((uint8_t*)("\r\nTIMER2\r\n"));//For test


}

void TIM3_IRQHandler(void)
{
	if((TIM3->SR & 0x0001) == 0x0001)
	{
		TIM3->SR &= ~(0x0001);
		timer3.countTicks++;

		// avoid overflow
		if((timer3.countTicks == MAX_COUNT) || (timer3.countTicks < 0)){
			timer3.countTicks = 0;
		}

	}
	// USART2_write((uint8_t*)("\r\nTIMER3\r\n"));//For test


}

void TIM4_IRQHandler(void)
{
	if((TIM4->SR & 0x0001) == 0x0001)
	{
		TIM4->SR &= ~(0x0001);
		timer4.countTicks++;

		// avoid overflow
		if((timer4.countTicks == MAX_COUNT) || (timer4.countTicks < 0)){
			timer4.countTicks = 0;
		}

	}
	// USART2_write((uint8_t*)("\r\nTIMER4\r\n"));//For test

}
