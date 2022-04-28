/*****************************************************************************/
/* Button main file, with interrupts                                         */
/* All references to the reference manual are to STM's RM0316, revision 8    */
/*****************************************************************************/




#include <catch.h>
#include <led.h>
#include <uart.h>
#include "stm32f303xe.h"
#include "types.h"
#include "wifi.h"
#define size_of_queue 10

enum led_state{off,on,blink};
enum event{connect_wifi,push_button,change_led_state};

int event_queue[size_of_queue];
int count_read=0;
int count_write=0;
int *p_read=event_queue;
int *p_write=event_queue;




int main(void)
{
	uartProccessorInit();
	uartComputerInit();
	sendCommand("AT+CIPMUX=1\r\n"); //configure server
	sendCommand("AT+CIPSERVER=1,80\r\n"); // configure server
	sendCommand("AT+CIFSR\r\n");








    while(1)
    {
    	/*
    	while(p_read!=p_write) // in case buffer is not empty
    	{
    		                   //handle an event in the buffer

    		if(count_read<size_of_queue-1) // in case its not the last place in the array
    			{
    		     	 p_read++;
    		     	 count_read++;
    			}
    		else {                         // in case its the last place in the array
        	     	 p_read=event_queue;
        	     	 count_read=0;

    	     	 }
    	}

    }
}

*/
    }
}
