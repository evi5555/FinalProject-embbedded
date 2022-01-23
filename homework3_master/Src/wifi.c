#include "catch.h"
#include <string.h>
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "led.h"
#include "stm32f303xe.h"

static char name_of_wifi[20];
static char password_of_wifi[20];
static char config_one[ ]={'A','T','+','C','W','M','O','D','E','=','1','\r','\n','\0'};
static char config_two[ ]={'A','T','+','C','W','J','A','P','=','\0'};
int counter=0;
int count_dump=0;
char dump[100];




void wifiConnect()
{
	//getting the name and password of Local Wifi
	//name_of_wifi[counter]='"';
	//counter++;
	print("please type the name of the wifiii now\n");
	while(1/*USART2->RDR!='\n'*/)  // that means that the last char received
	{
		//print("mamba in\n");
		if(USART2->RDR=='\n')
		{
			//name_of_wifi[counter]=USART2->RDR;
			//while(!(USART2->ISR & 0x00000020));  // check that data recevied
			break;
		}
		name_of_wifi[counter]=USART2->RDR;
		while(!(USART2->ISR & 0x00000020));  // check that data recevied
		//print("mamba in\n");
		counter++;
		count_dump++;

	}




	//while(!(USART2->ISR & 0x00000020));  // check that data recevied

	name_of_wifi[0]='"';
	name_of_wifi[counter]='"';         // demands of the wifi modem
	name_of_wifi[counter+1]=',';       // demands of the wifi modem
	counter=0;
	password_of_wifi[counter]='"';
	//dump=USART2->RDR;
	//while(!(USART2->ISR & 0x00000020));  // check that data recevied
	//USART2->RDR|=0xFFFFFFFF;
	//USART2->RDR |='A';

	print("please type the password of the wifi\n");
	count_dump=0;

	while(/*USART2->RDR!='\n'*/1)  // that means that the last char received
		{
			if(count_dump==1&&USART2->RDR=='\n')
			{
				break;
			}
			count_dump=1;
			print("im here\n");
			password_of_wifi[counter]=USART2->RDR;
			while(!(USART2->ISR & 0x00000020));  // check that data recevied
			counter++;

		}
	//print("im here\n");
	password_of_wifi[counter]='"';
	password_of_wifi[counter+1]='\r';       // demands of the wifi modem
	password_of_wifi[counter+2]='\n';		// demands of the wifi modem
	counter=0;


	//Connecting the Wifi

	while(config_one[counter]!='\0')
	{
		USART1->TDR=config_one[counter];  // input command 'AT+CWMODE=1'
		while(!(USART1->ISR & 0x00000080));
		counter++;


	}
	counter=0;
	while(config_two[counter]!='\0')
	{
		USART1->TDR=config_one[counter];  // input command 'AT+CWJAP='
		while(!(USART1->ISR & 0x00000080));
		counter++;

	}
	counter=0;

	while(name_of_wifi[counter]!=',')
	{
	   USART1->TDR=name_of_wifi[counter];      //input the name of the wifi
	   while(!(USART1->ISR & 0x00000080));
	   counter++;

	}

	USART1->TDR=name_of_wifi[counter]; // input the last char of the name of the wifi
	while(!(USART1->ISR & 0x00000080));
	counter=0;

	while(password_of_wifi[counter]!='\n')
	{
		USART1->TDR=password_of_wifi[counter]; // input the password of the wifi
		while(!(USART1->ISR & 0x00000080));
		counter++;

	}

	USART1->TDR=password_of_wifi[counter]; // input the last char of the password of the wifi
	while(!(USART1->ISR & 0x00000080));
	counter=0;

	while(USART1->RDR!='\n')    // printing the answer from the modem
	{
		print("%c\n",USART1->RDR);
		while(!(USART1->ISR & 0x00000020));  // check that data recevied
	}



}

void wifiLedExecute()
{

}
