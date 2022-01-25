
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
static char config_two[30]={'A','T','+','C','W','J','A','P','=','\0'};
static char at[8]={'A','T','E','0','\r','\n','\0'};
static char wifi_buffer[200];
int count_at=0;
int counter=1;
int count_dump=0;
int count_wifi_buffer=0;
int count_wifi_buftwo=0;
char dump[100];
char temp_c;
char temp_uart;
static int flag_name=0;
static int flag_password=0;
char *ret;




void wifiDetail()
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
		wifiConnect(name_of_wifi);
	}




}

void printResponse()
{

	while(ret==NULL)                             // check if there is '\n' means that command over
		{
			ret=memchr(wifi_buffer,'\n',200);
		}
	while(wifi_buffer[count_wifi_buftwo]!='\r'&&wifi_buffer[count_wifi_buftwo+1]!='\n')     // print the command
		{
			print("%c",wifi_buffer[count_wifi_buftwo]);
			count_wifi_buftwo++;
			if(wifi_buffer[count_wifi_buftwo]=='\n')
			{
				wifi_buffer[count_wifi_buftwo]=' ';
			}

		}
	wifi_buffer[count_wifi_buftwo+1]=' ';


	count_wifi_buftwo++;
	ret=NULL;


}
void wifiConnect(char* wifi_arr)
{
	while(at[count_at]!='\0')
	{
		USART1->TDR=at[count_at];        // input command 'ATE0'
		while(!(USART1->ISR & 0x00000080));
		count_at++;
	}
	printResponse();

	strcat(config_two,wifi_arr);
	//print("%s",config_two);
	while(config_one[counter]!='\0')
		{
			USART1->TDR=config_one[counter];  // input command 'AT+CWMODE=1'
			while(!(USART1->ISR & 0x00000080));
			counter++;

		}
	printResponse();
	counter=0;

	while(config_two[counter]!='\0')
		{
			USART1->TDR=config_two[counter];  // input command 'AT+CWJAP=wifi details'
			while(!(USART1->ISR & 0x00000080));
			counter++;
		}
	printResponse();
	printResponse();
	printResponse();
	printResponse();
	printResponse();
	printResponse();
	printResponse();
	//printResponse();

		counter=0;


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
	    wifiDetail();
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
		}
		else if(temp_c!='\n'&&flag_name==0)
		{
			name_of_wifi[counter]=temp_c;
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
			//print("%s",name_of_wifi);
			//print("%s",password_of_wifi);
		}

}

void USART1_EXTI25_IRQHandler(void)   // interrupt uart for proccessor
{

		wifi_buffer[count_wifi_buffer]=USART1->RDR;
		count_wifi_buffer++;








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


