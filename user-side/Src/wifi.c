#include "catch.h"
#include <string.h>
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "led.h"
#include "stm32f303xe.h"

static char config_one[ ]={'A','T','+','C','W','M','O','D','E','=','1','\r','\n','\0'};
static char config_two[30]={'A','T','+','C','W','J','A','P','=','\0'};
static char at[8]={'A','T','E','0','\r','\n','\0'};
//int counter=0;
//int count_dump=0;
char dump[100];
char temp_c;
int count=0;








void wifiConnect(char* wifi_arr)
{
	sendCommand(at); // input command 'ATE0'

	strcat(config_two,wifi_arr);  // union between wifi details and 'AT+CWJAP COMMAND

	sendCommand(config_one); // input command 'AT+CWMODE=1'
	count=0;

	sendCommand(config_two); // input command 'AT+CWJAP=wifi details'
	printResponse();      // print response
	printResponse();
	printResponse();
	printResponse();
	printResponse();
	printResponse();


		count=0;


}




void wifiLedExecute()
{

}
