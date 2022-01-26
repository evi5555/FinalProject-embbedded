/*****************************************************************************/
/* Button main file                                                          */
/* All references to the reference manual are to STM's RM0316, revision 8    */
/*****************************************************************************/




#include "stm32f303xe.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

static char A_Buffer[128];



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





int main(void)
{
    // Enable GPIOA clock (p. 148 in the reference manual).
    RCC->AHBENR |= 0x000A0000;
    // Configure GPIOA pin 5 as output
    // (by default it will then be push pull, see p. 237 of the reference manual).
    GPIOA->MODER |= 0x00000400;
    // GPIOA pin 0 is connected to the motion sensor.
    // No need to configure it, because by default it's an input.

    while(1)
    {
    	if(GPIOA->IDR & 0x00000001)
        {
            GPIOA->ODR |= 0x00000020;
            print("there is a movement in the house,please check!\n");
        }
    	else
    	{
    		GPIOA->ODR &= ~0x00000020;
    	}
    }
}
