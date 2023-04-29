/*
 * IRIG.c
 *
 *  Created on: Apr 29, 2023
 *      Author: marti
 */

#include "IRIG.h"
#include "stdbool.h"
#include "di.h"


bool mPumpAState;
bool mPumpBState;
bool mIrigValveState;


void StartPumpA(void);
void StopPumpA(void);
void StartPumpB(void);
void StopPumpB(void);
void OpenIrigValve(void);
void CloseIrigValve(void);


void IRIG_Init(void)
{
  // TBD
}


void IRIG_Update_1s(void)
{
  bool tankFull;
  bool tankEmpty;
  bool wellAEmpty;

  // read the level bi-state sensors

  tankFull = DI_Get(IN1_LEVEL1_TANKFULL);
  tankEmpty = DI_Get(IN2_LEVEL2_TANKEMPTY);
  wellAEmpty = DI_Get(IN3_LEVEL3_WELLAEMPTY);



  // automatic refilling of irrigation tank from well (gray water sewage)
  if(!tankFull && !wellAEmpty)
  {
    StartPumpA();
  }
  else
  {
    StopPumpA();
  }

}


void StartPumpA(void)
{
  HAL_GPIO_WritePin(REL1_GPIO_Port, REL1_Pin, GPIO_PIN_RESET);
  mPumpAState = true;
}

void StopPumpA(void)
{
  HAL_GPIO_WritePin(REL1_GPIO_Port, REL1_Pin, GPIO_PIN_SET);
  mPumpAState = false;
}


void StartPumpB(void)
{
  HAL_GPIO_WritePin(REL2_GPIO_Port, REL2_Pin, GPIO_PIN_RESET);
  mPumpBState = true;
}

void StopPumpB(void)
{
  HAL_GPIO_WritePin(REL2_GPIO_Port, REL2_Pin, GPIO_PIN_SET);
  mPumpBState = false;
}

void OpenIrigValve(void)
{
  HAL_GPIO_WritePin(REL3_GPIO_Port, REL3_Pin, GPIO_PIN_RESET);
  mIrigValveState = true;
}

void CloseIrigValve(void)
{
  HAL_GPIO_WritePin(REL3_GPIO_Port, REL3_Pin, GPIO_PIN_SET);
  mIrigValveState = false;
}


