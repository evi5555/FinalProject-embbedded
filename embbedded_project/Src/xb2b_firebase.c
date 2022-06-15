

#include "uart.h"
#include <string.h>
#include <stdio.h>
#include "timer.h"

#include "led.h"


#define COMMAND_SIZE 256
#define HTTP_SIZE 512
#define CONTENT_SIZE 128
#define PATH_SIZE 128

static STATE found = STANDBY;
static uint8_t command[COMMAND_SIZE];

static uint8_t http[HTTP_SIZE];
static uint32_t http_len = 0 ;

static uint8_t content[CONTENT_SIZE]; //The HTTP body
static uint32_t content_len = 0 ;

static uint8_t image_path[PATH_SIZE];

static BOOL connection_closed;
static SWITCH_STATE state;

uint8_t SSID[WiFi_SIZE]; // extern see configurations
uint8_t PWD[WiFi_SIZE];  // extern see configurations
uint8_t DEVICE_ID[ID_SIZE]; // extern see configurations


BOOL connectFirebaseHost(uint32_t _ssl_tries, uint32_t _cipstart_tries , uint32_t _ssl_timeout, uint32_t  _cipstart_timeout){


	//Create secure cunnection via SSL
	found = STANDBY;
	USART1_write((uint8_t*)"AT+CIPSSLSIZE=4096\r\n");//at_instruction: 5.2.4 page 50
	while(_ssl_tries > 0){
		TIMER4_set_timeout(_ssl_timeout);
		while(found == STANDBY && !TIMER4_timeout_done()){
			if(USART1_NEW_LINE_FOUND_get()){
				found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
				USART1_NEW_LINE_READ_set();
			}
		}
		if(found == PASS){
			break; //move on to next command
		}
		else{ // FAIL OR TIMEOUT
			_ssl_tries--;
			found = STANDBY; // reset found
			USART1_write((uint8_t*)"AT+CIPSSLSIZE=4096\r\n");
		}
	}

	if(found == FAIL || found == STANDBY){
		return FALSE;
	}



	//Connect Firebase Host
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSTART=\"SSL\",\"%s\",%ld\r\n",(char*)firebase_host, https_port);

	found = STANDBY;
	USART1_write((uint8_t*)command);
	while(_cipstart_tries > 0){
		TIMER4_set_timeout(_cipstart_timeout);
		while(found == STANDBY && !TIMER4_timeout_done()){
			if(USART1_NEW_LINE_FOUND_get()){
				found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
				if(found == STANDBY){
					found = USART1_search_buffer_Rx((uint8_t *)AT_ALREADY_CONNECTED, (uint8_t *)AT_ERROR);// CRITICAL!
				}
				USART1_NEW_LINE_READ_set();
			}
		}
		if(found == PASS){
			return TRUE;
		}
		else{ // FAIL OR TIMEOUT
			_cipstart_tries--;
			found = STANDBY; // reset found
			USART1_write((uint8_t*)command);
		}
	}
	return FALSE;



}
