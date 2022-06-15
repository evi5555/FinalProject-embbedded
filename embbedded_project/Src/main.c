/*****************************************************************************/
/* Button main file, with interrupts                                         */
/* All references to the reference manual are to STM's RM0316, revision 8    */
/*****************************************************************************/




#include <catch.h>
#include <led.h>
#include <uart.h>
#include <xb2b_wifi.h>
#include "stm32f303xe.h"
#include "types.h"
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
	//set_sys_clock_to_32MHz();
	//init_MCO();




	uartProccessorInit(); // for xb2b
	uartComputerInit(); // for debugging

	TIMER2_init(); // for monitoring switch state.
	TIMER3_init(); // for sensor delay
	TIMER4_init(); // for xbee timeout

	QUEUE_init();

	CONFIGURATIONS_set_device_id();




	TIMER4_set_timeout(60);
	while(QUEUE_isEmpty() && !TIMER4_timeout_done());

	SENSOR_init(); // sensor interrupts are not inabled

	LED_init();



	//init_i2c1();
	sendCommand((uint8_t*)("\r\n_______________\r\n"));//For test

	while(1)
	{

		QUEUE_do_event();
		SYSTEM_CONTROL_monitor_switch_state(120); // every 2 minutes

	}
}
