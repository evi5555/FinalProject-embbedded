
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "stm32f303xe.h"
#include "types.h"
#include "uart.h"
#include "led.h"
#include "catch.h"
#include "wifi.h"
// This buffer is used by the printf-like print function.
static char A_Buffer[USART2_SIZE_OF_PRINT_BUFFER];





void uartProccessorInit()
{

	// Enable GPIOC clock (p. 148 in the datasheet).
	        RCC->AHBENR |= 0x00080000;
	    // Enable USART1 clock.
	        RCC->APB2ENR |= 0x00004000;
	    // Configure GPIOC pins 4 and 5 as alternate function 7, which is USART1.
	        GPIOC->MODER |= 0x00000A00;
	        GPIOC->AFR[0]|= 0x00770000;
	        USART1->BRR = 69;  // 115200 wifi baud rate
	        // Enable USART1 and its TX functionality.
	        // Enable USART1 and its RX and TX functionality.
	        // Also enable the RX interrupt.
	        USART1->CR1 = 0x0000002D;
	        NVIC_EnableIRQ(USART1_IRQn);


}

void uartComputerInit()
{
	// Enable GPIOA clock (p. 148 in the datasheet).
	    RCC->AHBENR |= 0x00020000;
	    // Enable USART2 clock.
	    RCC->APB1ENR |= 0x00020000;
	    // Configure GPIOA pins 2 and 3 as alternate function 7, which is USART2.
	    GPIOA->MODER |= 0x000000A0;
	    GPIOA->AFR[0] |= 0x00007700;
	    USART2->BRR = 833;   // 9600 baud rate
	    // Enable USART2 and its TX functionality.
	    // Enable USART2 and its RX and TX functionality.
	    // Also enable the RX interrupt.
	    USART2->CR1 = 0x0000002D;
	    wifiConnect();
	    NVIC_EnableIRQ(USART2_IRQn);
}


void USART2_EXTI26_IRQHandler(void)  // interrupt uart for proccessor
{


}

void USART1_EXTI25_IRQHandler(void)   // interrupt uart for computer
{




}

void print(char *p_format, ...)
{
	va_list p_variables;
	va_start(p_variables, p_format);
	(void)vsprintf(A_Buffer, p_format, p_variables);
    USART2_print(A_Buffer);
}


void USART2_print(const char *p_data)
{
	while(*p_data != '\0')
	{
		USART2->TDR = *p_data;
        p_data++;
        while(!(USART2->ISR & 0x00000080));
	}
}


