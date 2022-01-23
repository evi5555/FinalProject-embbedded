/******************************************************************************
Scheduler
  Handles the scheduled tasks in the system.
  This includes blinking the LED, after a command to do so is received,
  and printing to the screen periodically.
  The scheduler uses Timer 2 to measure time intervals.
******************************************************************************/
#ifndef SCHEDULER_H_
#define SCHEDULER_H_





/******************************************************************************
Definitions
******************************************************************************/
#define SCHEDULER_BLINK_HALF_PERIOD_IN_SEC 5
#define SCHEDULER_BORING_PERIOD_IN_SEC 60




/******************************************************************************
SCHEDULER_blink
Description:
  Start blinking the LED.
  The LED will with a period of twice SCHEDULER_BLINK_HALF_PERIOD_IN_SEC.
******************************************************************************/
void SCHEDULER_blink(void);

/******************************************************************************
SCHEDULER_handle
Description:
  Handle the expiration of Timer 2 (once a second).
  Execute timed tasks if relevant.
******************************************************************************/
void SCHEDULER_handle(void);

/******************************************************************************
SCHEDULER_stopBlinking
Description:
  Stop blinking the LED.
******************************************************************************/
void SCHEDULER_stopBlinking(void);




#endif /* SCHEDULER_H_ */
