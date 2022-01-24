#ifndef LED_H_
#define LED_H_




/******************************************************************************
Includes
******************************************************************************/
#include "stm32f303xe.h"


void LedInit();


void EXTI15_10_IRQHandler(void);



#endif /* LED_H_ */
