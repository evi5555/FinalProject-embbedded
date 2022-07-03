

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




	BOOL joinAccessPoint(uint32_t tries, uint32_t timeout){

		memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
		sprintf((char*)command, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PWD);

		found = STANDBY;
		USART1_write((uint8_t*)command);
		while(tries > 0){
			TIMER4_set_timeout(timeout);
			while(found == STANDBY && !TIMER4_timeout_done()){
				if(USART1_NEW_LINE_FOUND_get()){
					found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
					USART1_NEW_LINE_READ_set();
				}
			}
			if(found == PASS){
				return TRUE;
			}
			else{ // FAIL OR TIMEOUT
				tries--;
				found = STANDBY; // reset found
				USART1_write((uint8_t*)command);
			}
		}
		return FALSE;

	}


	/*This function cunnects to firebase via secure HTTP (HTTPS) using SSL,
	 * returns TRUE upon success.
	 * tries: number of times to send ping incase of timeout or failure.
	 * timeout (in seconds): number of seconds to wait for response
	 * need to enter tries and timout for both SSL AT_command and CIPSTART AT_command*/
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


	/*This function builds HTTP request to send to firebase,
	 * Returns when done*/
	void createPostMsg(void){

		//Set HTTP body content
		memset((char*)content, '\0', CONTENT_SIZE*sizeof(uint8_t));
		sprintf((char*)content,"{\"image_path\": \"image/path\", \"notes\": \"alarm went off!\", \"timestamp\": {\".sv\": \"timestamp\"}}");
		content_len = strlen((char*)content);

		//Set HTTP request
		memset((char*)http, '\0', HTTP_SIZE*sizeof(uint8_t));
		sprintf((char*)http,"POST /devices/%s/history.json?auth=%s HTTP/1.0\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n%s\r\n",(char*)DEVICE_ID,(char*)firebase_auth_key,(char*)firebase_host,content_len,(char*)content); // HTTP/1.0- Allow only one request
		//sprintf((char*)http,"POST /devices/%s/history.json?auth=%s&print=silent HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n{\"image_path\": \"%s\", \"notes\": \"alarm went off\", \"timestamp\": {\".sv\": \"timestamp\"}}\r\n",(char*)DEVICE_ID,(char*)firebase_auth_key,(char*)firebase_host,content_len,(char*)image_path); // HTTP/1.0- Allow only one request
		http_len = strlen((char*)http)-strlen("\r\n"); // the last \r\n is for the AT command, and not included in the request's length

	}

	void createGetMsg(void){

		//Set HTTP request
		memset((char*)http, '\0', HTTP_SIZE*sizeof(uint8_t));
		sprintf((char*)http,"GET /devices/%s/control/state.json?auth=%s HTTP/1.0\r\nHost: %s\r\n\r\n\r\n",(char*)DEVICE_ID,(char*)firebase_auth_key,(char*)firebase_host); // HTTP/1.0- Allow only one request
		http_len = strlen((char*)http)-strlen("\r\n"); // the last \r\n is for the AT command, and not included in the request's length

	}

	void createPutMsg(void){

		//Set HTTP body content
		memset((char*)content, '\0', CONTENT_SIZE*sizeof(uint8_t));
		sprintf((char*)content,"{\"is_in_use\": true}");
		content_len = strlen((char*)content);

		//Set HTTP request
		memset((char*)http, '\0', HTTP_SIZE*sizeof(uint8_t));
		sprintf((char*)http,"PUT /device-list/%s.json?auth=%s HTTP/1.0\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n%s\r\n",(char*)DEVICE_ID,(char*)firebase_auth_key,(char*)firebase_host,content_len,(char*)content); // HTTP/1.0- Allow only one request
		http_len = strlen((char*)http)-strlen("\r\n"); // the last \r\n is for the AT command, and not included in the request's length
	}


	void TestWifiConnection(void){


		// before useing this function init_usart1(); and  init_usart2(); must be executed





		//Set client mode
		USART1_write((uint8_t*)AT_CWMODE);
		USART2_write((uint8_t*)"BEFORE AT_CWMODE WHILE\r\n");
		while(!found){
			found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
		}
		USART2_write((uint8_t*)"AT_CWMODE PASSED\r\n");
		found = FALSE;


		//Join access point
		memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
		sprintf((char*)command, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);

		USART1_write((uint8_t*)command);

		while(!found){
			found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
		}
		found = FALSE;



		/*Default: AT+CIPMUX=0 (according to: AT instruction set- 5.2.15)*/

		//Connect to API
		memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
		sprintf((char*)command, "AT+CIPSTART=\"TCP\",\"%s\",80\r\n", (char*)weather_api);

		//USART2_write((uint8_t*)command); // test
		USART1_write((uint8_t*)command);

		while(!found){
			found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
			found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ALREADY_CONNECTED);
		}
		//USART2_write((uint8_t*)"AT+CIPSTART PASSED\r\n");
		found = FALSE;


		//Set HTTP request
		memset((char*)http, '\0', HTTP_SIZE*sizeof(uint8_t));
		sprintf((char*)http,("GET /data/2.5/weather?q=London,uk&appid=%s HTTP/1.0\r\nHost: %s\r\n\r\n\r\n"),weather_api_key,weather_api); // HTTP/1.0- Allow only one request
		http_len = strlen((char*)http)-strlen("\r\n"); // the last \r\n is for the AT command, and not included in the request's length

		//Send number of data bytes
		memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
		sprintf((char*)command, "AT+CIPSEND=%ld\r\n",http_len);


		//USART2_write((uint8_t*)command); // test
		USART1_write((uint8_t*)command);

		while(!found){
			found = USART1_search_buffer_Rx((uint8_t *)">", (uint8_t *)AT_ERROR);
		}
		found = FALSE;

		USART1_write((uint8_t*)http);

		// SEND OK
		while(!found){
			found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
		}
		found = FALSE;

		// READ RESPONSE
		while(!found){
			//found = USART1_search_buffer_Rx((uint8_t *)"\r\n\r\nOK\r\n", (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
			found = USART1_search_buffer_Rx((uint8_t *)"CLOSED\r\n", (uint8_t *)AT_FAIL);
		}
		found = FALSE;

		//Close TCP connection: THIS MIGHT BE UNESESARY BECAUSE HTTP is vs 1.0 wich allows one request
		USART1_write((uint8_t*)AT_CIPCLOSE);
		while(!found){
			found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
		}
		found = FALSE;


	BOOL closeConnection(uint32_t tries, uint32_t timeout){

		found = STANDBY;
		USART1_write((uint8_t*)AT_CIPCLOSE);
		while(tries > 0){
			TIMER4_set_timeout(timeout);
			while(found == STANDBY && !TIMER4_timeout_done()){
				if(USART1_NEW_LINE_FOUND_get()){
					found = USART1_search_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
					USART1_NEW_LINE_READ_set();
				}
			}
			if(found == PASS){
				return TRUE;
			}
			else{ // FAIL OR TIMEOUT
				tries--;
				found = STANDBY; // reset found
				USART1_write((uint8_t*)AT_CIPCLOSE);
			}
		}
		return FALSE;

	}


	BOOL parseResponse(uint32_t timeout){

		found = STANDBY;
		state = NON;
		TIMER4_set_timeout(timeout);
		while(found == STANDBY && !TIMER4_timeout_done()){
			if(USART1_NEW_LINE_FOUND_get()){
				state = USART1_check_state_buffer_Rx((uint8_t *)"\"on\"CLOSED", (uint8_t *)"\"off\"CLOSED", (uint8_t *)"nullCLOSED");
				if(state != NON){
					found = USART1_search_buffer_Rx((uint8_t *)"CLOSED\r\n", (uint8_t *)AT_FAIL);
				}
				USART1_NEW_LINE_READ_set();
			}
		}
		if(found == PASS){
			return TRUE;
		}
		else{
			return FALSE;
		}


	}


	BOOL sendRequest(uint32_t _CIPSEND_tries,uint32_t _SEND_OK_tries , uint32_t _CIPSEND_timeout, uint32_t _SEND_OK_timeout ){


		/*Send Request Length - number of data bytes to be sent*/
		memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
		sprintf((char*)command, "AT+CIPSEND=%ld\r\n",http_len);

		found = STANDBY;
		USART1_write((uint8_t*)command);
		while(_CIPSEND_tries > 0){
			TIMER4_set_timeout(_CIPSEND_timeout);
			while(found == STANDBY && !TIMER4_timeout_done()){
				if(USART1_NEW_LINE_FOUND_get()){
					found = USART1_search_buffer_Rx((uint8_t *)">", (uint8_t *)AT_ERROR);
					if(found == STANDBY){
						found = USART1_search_buffer_Rx((uint8_t *)">", (uint8_t *)"CLOSED\r\n");
					}
					USART1_NEW_LINE_READ_set();
				}
			}
			if(found == PASS){
				break; //move on to next command
			}
			else{ // FAIL OR TIMEOUT
				_CIPSEND_tries--;
				found = STANDBY; // reset found
				USART1_write((uint8_t*)command);
			}
		}
		if(found == FAIL || found == STANDBY){
			return FALSE;
		}


		/*Send HTTP request*/
		found = STANDBY;
		USART1_write((uint8_t*)http);

		/*Wait for SEND_OK after this a response will come*/
		while(_SEND_OK_tries > 0){
			TIMER4_set_timeout(_SEND_OK_timeout);
			while(found == STANDBY && !TIMER4_timeout_done()){
				if(USART1_NEW_LINE_FOUND_get()){
					found = USART1_search_buffer_Rx((uint8_t *)SEND_OK, (uint8_t *)AT_FAIL);
					USART1_NEW_LINE_READ_set();
				}
			}
			if(found == PASS){
				return TRUE;
			}
			else{
				_SEND_OK_tries--;
				found = STANDBY; // reset found
				USART1_write((uint8_t*)http);
			}
		}
		return FALSE;

	}

}
