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


void IRIG_Init(void);

void IRIG_Update_1s(void);

void IRIG_SetupAutoIrrig(uint16_t hour1, uint16_t hour2, uint16_t duration);

void IRIG_SetAutoMode(void);

void IRIG_IrrigateNow(uint16_t duration);

#endif /* INC_IRIG_H_ */
