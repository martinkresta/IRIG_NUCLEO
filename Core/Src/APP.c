/*
 * APP.c
 *
 *  Created on: Aug 14, 2021
 *      Author: Martin
 *      Brief:  Main application and compile time configuration
 *
 */

#include "RTC.h"
#include "UI.h"
#include "main.h"
#include "APP.h"
#include "scheduler.h"
#include "GEST.h"
#include "di.h"
#include "IRIG.h"
#include "SONAR.h"
#include "RADIO.h"




// public methods
void APP_Init(void)
{

	sUIHwInit uihw;
	uint8_t gi = 0;   //gesture index
	sGestInit gestInit;

	Scheduler_Init();

	DI_Init();
	IRIG_Init();
	RADIO_Init(&hlpuart1);
	SONAR_Init();



	/*Assign pins for onboard UI  */
	uihw.Led_Life.Pin = LED_Life_Pin;
	uihw.Led_Life.Port = LED_Life_GPIO_Port;
	uihw.Led_Life.Logic = eUIL_POSITIVE;

	uihw.Led_R.Pin = LED_R_Pin;
	uihw.Led_R.Port = LED_R_GPIO_Port;
	uihw.Led_R.Logic = eUIL_POSITIVE;

	uihw.Led_G.Pin = LED_G_Pin;
	uihw.Led_G.Port = LED_G_GPIO_Port;
	uihw.Led_G.Logic = eUIL_POSITIVE;

	uihw.Led_B.Pin = LED_G_Pin;          // NOT AVAIABLE, DO NOT USE!
	uihw.Led_B.Port = LED_G_GPIO_Port;
	uihw.Led_B.Logic = eUIL_POSITIVE;

	uihw.Buzzer.Pin = LED_G_Pin;         // NOT AVAIABLE, DO NOT USE!
	uihw.Buzzer.Port = LED_G_GPIO_Port;
	uihw.Buzzer.Logic = eUIL_POSITIVE;

	UI_Init(&uihw);
	UI_LED_Life_SetMode(eUI_BLINKING_SLOW);
	UI_LED_B_SetMode(eUI_ON);


	/*Gestures definition*/

	  gestInit.id = gi++;
    gestInit.action = GEST_IRIG_DUAL;
    gestInit.type = egt_MultiTouch;
    gestInit.num_of_buttons = 1;
    gestInit.timeout = 500;
    gestInit.num_of_touches = 2;
    gestInit.btnIds[0] = IN4_BTN1;
    GEST_AddGesture(&gestInit);

	  gestInit.id = gi++;
	  gestInit.action = GEST_IRIG_TRIPLE;
	  gestInit.type = egt_MultiTouch;
	  gestInit.num_of_buttons = 1;
	  gestInit.timeout = 500;
	  gestInit.num_of_touches = 3;
	  gestInit.btnIds[0] = IN4_BTN1;
	  GEST_AddGesture(&gestInit);
}

void APP_Start(void)
{

  // SET RTC time
  sDateTime now;
  now.Hour = 21;
  now.Minute = 38;
  now.Second = 0;

  RTC_SetTime(now);


	while (1)   // endless loop
	{
		Scheduler_Check_Flag();

	}
}

void APP_Update_1s(void)
{
	// check midnight
	static uint8_t dayNumber = 0;
	uint8_t newDayNumber = 0;
	newDayNumber = RTC_GetTime().Day;
	if (dayNumber != newDayNumber)
	{
		dayNumber = newDayNumber;

		// Reset counters

	}
}



// map the digital input to the button ID and propagate the information
void APP_DiInputChanged(uint8_t inputId, eDI state)
{
  switch (inputId)
  {
    case IN1_LEVEL1_TANKFULL:
      break;
    case IN2_LEVEL2_TANKEMPTY:
      break;
    case IN3_LEVEL3_WELLAEMPTY:
      break;
    case IN4_BTN1:
      break;
    case IN5_BTN2:
      break;
  }
}

void APP_GestureDetected(uint8_t action)
{
  switch(action)
  {
    case GEST_IRIG_DUAL:
      break;
    case GEST_IRIG_TRIPLE:
      break;
  }
}


