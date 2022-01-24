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
//int counter=0;
//int count_dump=0;
char dump[100];
char temp_c;







void wifiLedExecute()
{

}
