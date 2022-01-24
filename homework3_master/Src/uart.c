
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
static char name_of_wifi[40];
static char password_of_wifi[40];
static char config_one[ ]={'A','T','+','C','W','M','O','D','E','=','1','\r','\n','\0'};
static char config_two[ ]={'A','T','+','C','W','J','A','P','=','\0'};
int counter=1;
int count_dump=0;
char dump[100];
char temp_c;
static int flag_name=0;
static int flag_password=0;




void wifiConnect()
{
	if(flag_name==0&&flag_password==0)
	{
		name_of_wifi[0]='"';
		password_of_wifi[0]='"';
		print("please type the name of the wifi now\n");
		while(flag_name!=1);
	}
	 if(flag_name==1&&flag_password==0)
	{
		print("please type the password of the wifi\n");
		while(flag_password!=1);
		strcat(name_of_wifi,password_of_wifi);
	}




}





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
	    NVIC_EnableIRQ(USART2_IRQn);
	    wifiConnect();
}


void USART2_EXTI26_IRQHandler(void)  // interrupt uart for computer
{
		temp_c=USART2->RDR;
		if(temp_c=='\n'&&flag_name==0)
		{
			flag_name=1;
			name_of_wifi[counter]='"';         // demands of the wifi modem
			name_of_wifi[counter+1]=',';       // demands of the wifi modem
			counter=1;
			print("elnini\n%c",temp_c);
		}
		else if(temp_c!='\n'&&flag_name==0)
		{
			name_of_wifi[counter]=temp_c;
			print("%c",temp_c);
			counter++;
		}
		else if(temp_c!='\n'&&flag_name==1) // that means that we need to configure the password
		{
			password_of_wifi[counter]=temp_c;
			counter++;
		}
		else if(temp_c=='\n'&&flag_name==1)  // that means we finish configure password
		{
			flag_password=1;
			password_of_wifi[counter]='"';
			password_of_wifi[counter+1]='\r';       // demands of the wifi modem
			password_of_wifi[counter+2]='\n';		// demands of the wifi modem
			counter=0;
			print("%s",name_of_wifi);
			print("%s",password_of_wifi);
		}

}

void USART1_EXTI25_IRQHandler(void)   // interrupt uart for proccessor
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


