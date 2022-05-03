#include "catch.h"
#include <string.h>
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f303xe.h"
#include "wifi.h"



void LedInit()
{
	// Enable GPIOA and GPIOC clocks.
	    RCC->AHBENR |= 0x000A0000;
//Enable the SYSCFG(for managing the external interrupt) clock.This is necessary for connecting PC13 to EXTI13.
	    RCC->APB2ENR |= 0x00000001;
	    // Configure PA5 as output
	        // (by default it will then be push pull, see p. 237 of the reference manual).
	    GPIOA->MODER |= 0x00000400;

	    // Configure EXTI(extended interrupt) 13, to which PC13 can be connected:
	        // Connect PC13 to EXTI13. See p. 252 of the reference manual.
	        // Note that the reference manual counts the registers beginning with EXTICR from
	        // 1 to 4,
	        // while the h-file stm32f303xe.h defines the registers as an array of size 4.
	        // So EXTICR4 in the reference manual is EXTICR[3] in the program.
	    SYSCFG->EXTICR[3] |= 0x00000020;
	    // Set EXTI13 to be triggered by a falling edge.
	    EXTI->FTSR |= 0x00002000;
	    // Enable EXTI13.
	    EXTI->IMR |= 0x00002000;
	        // Enable the interrupt handler for EXTI13.
	    NVIC_EnableIRQ(EXTI15_10_IRQn);


}

void LED_toggle(){
	// Toggle GPIOA pin 5.
	GPIOA->ODR ^= 0x0020;
}

void LED_on()
{
	GPIOA->ODR |= 0x0020;
}

void LED_off()
{
	GPIOA->ODR &= ~(0x0020);
}

void EXTI15_10_IRQHandler(void)
{
	// Reset the interrupt request flag.
		EXTI->PR |= 0x00002000;


}


