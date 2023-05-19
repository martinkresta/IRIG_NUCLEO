/*
 * IRIG.h
 *
 *  Created on: Apr 29, 2023
 *      Author: marti
 */

#ifndef INC_IRIG_H_
#define INC_IRIG_H_

#include "main.h"
#include "stdbool.h"

typedef struct
{
  bool state;
  uint16_t minOnTime;
  uint16_t maxOnTime;
  uint16_t minOffTime;
  uint16_t stateTimer;
  uint32_t pin;
  GPIO_TypeDef* port;
}sOutputControl;


typedef struct
{
  uint16_t tankLevel_l;
  uint16_t pumpADailyOnTime_s;
  uint16_t dailyIrrigLiters_l;
  uint16_t statusFlags;
}sIrigStatus;

#define IRIG_STATFLAG_IRIGVALVE     0
#define IRIG_STATFLAG_PUMP_A        1
#define IRIG_STATFLAG_PUMP_B        2
#define IRIG_STATFLAG_AUTOIRRIG     4
#define IRIG_STATFLAG_FLUSHTANK     5
#define IRIG_STATFLAG_TANKFULL      6
#define IRIG_STATFLAG_WELLAEMPTY    7

void IRIG_Init(void);

void IRIG_Update_1s(void);

void IRIG_Midnight(void);

void IRIG_SetupAutoIrrig(uint16_t hour1, uint16_t hour2, uint16_t duration);

void IRIG_SetAutoMode(void);

void IRIG_IrrigateNow(uint16_t duration);

void IRIG_ToggleIrrigiation(void);

void IRIG_FlushTank(void);

void IRIG_FillTank(void);

#endif /* INC_IRIG_H_ */
