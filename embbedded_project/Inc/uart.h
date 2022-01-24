#ifndef UART_H_
#define UART_H_



#include "types.h"

#define USART2_SIZE_OF_PRINT_BUFFER 128



void uartComputerInit();

void uartProccessorInit();

void USART2_EXTI26_IRQHandler(void);



void print(char *p_format, ...);


void USART2_print(const char *p_data);

#endif /* USART2_H_ */