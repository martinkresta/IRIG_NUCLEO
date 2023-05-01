/*
 * IRIG.c
 *
 *  Created on: Apr 29, 2023
 *      Author: marti
 */

#include "IRIG.h"
#include "stdbool.h"
#include "di.h"
#include "RTC.h"


sOutputControl mPumpACtrl;
sOutputControl mPumpBCtrl;
sOutputControl mIrigValveCtrl;

bool mAutoIrrigation;

// irigation settings

uint16_t mIrigTime_s;
uint16_t mIrigHour1;
uint16_t mIrigHour2;

uint16_t mIrigDuration_s;


void StartOuptut(sOutputControl* out);
void StopOuptut(sOutputControl* out);


void IRIG_Init(void)
{
  mPumpACtrl.state = false;
  mPumpACtrl.minOffTime = 45*60;  // 45 minutes
  mPumpACtrl.minOnTime = 15;
  mPumpACtrl.maxOnTime = 15*60;  // 15 minutes
  mPumpACtrl.pin = REL1_Pin;
  mPumpACtrl.port = REL1_GPIO_Port;
  mPumpACtrl.stateTimer = 60000;

  mPumpBCtrl.state = false;
  mPumpBCtrl.minOffTime = 45*60;  // 45 minutes
  mPumpBCtrl.minOnTime = 15;
  mPumpBCtrl.maxOnTime = 15*60;  // 15 minutes
  mPumpBCtrl.pin = REL2_Pin;
  mPumpBCtrl.port = REL2_GPIO_Port;
  mPumpBCtrl.stateTimer = 60000;

  mIrigValveCtrl.state = false;
  mIrigValveCtrl.minOffTime = 1;  // 10 seconds
  mIrigValveCtrl.minOnTime = 1;
  mIrigValveCtrl.maxOnTime = 4* 60 *60;  // 4 hours
  mIrigValveCtrl.pin = REL3_Pin;
  mIrigValveCtrl.port = REL3_GPIO_Port;
  mIrigValveCtrl.stateTimer = 60000;


  mIrigTime_s = 2*60*60;  // 2 hour
  mIrigHour1 = 7;       // morning 7:00
  mIrigHour2 = 19;      // evening 7:00  // hint: to disable one irrigation period, set the hour to more than 24 ;-)

  mAutoIrrigation = true;
}


void IRIG_Update_1s(void)
{
  bool tankFull;
  bool tankEmpty;
  bool wellAEmpty;

  sDateTime now = RTC_GetTime();

  // read the level bi-state sensors

  tankFull = DI_Get(IN1_LEVEL1_TANKFULL);
  tankEmpty = DI_Get(IN2_LEVEL2_TANKEMPTY);
  wellAEmpty = DI_Get(IN3_LEVEL3_WELLAEMPTY);


  // automatic refilling of irrigation tank from well (gray water savage)
  if(!tankFull && !wellAEmpty)
  {
    // limit the pump start to every quarter past, to spread power supply load in time.
    // (The other savage pump is running always from XX:00 to XX:07)
    if(now.Minute == 15)
    {
      StartOuptut(&mPumpACtrl);
    }

  }
  else
  {
    StopOuptut(&mPumpACtrl);
  }


  // automatic irrigation

  if(mAutoIrrigation == true)
  {
    if (now.Hour == mIrigHour1 || now.Hour == mIrigHour2)
      {
        if(now.Minute == 00 && now.Second == 0)
        {
          mIrigDuration_s = mIrigTime_s;
          StartOuptut(&mIrigValveCtrl);
        }
      }
  }


  // stoping of irrigation (both manual and auto triggered)

  if(mIrigValveCtrl.state == true && (mIrigValveCtrl.stateTimer > mIrigDuration_s))
  {
    StopOuptut(&mIrigValveCtrl);
  }


  // apply output time limitations
  // increment the timers preventing overflow (maximal timing is 1000 minutes)
  if(mPumpACtrl.stateTimer < 60000) mPumpACtrl.stateTimer++;
  if(mPumpBCtrl.stateTimer < 60000) mPumpBCtrl.stateTimer++;
  if(mIrigValveCtrl.stateTimer < 60000) mIrigValveCtrl.stateTimer++;

  // check max ON time of all inputs
  if(mPumpACtrl.state == true && (mPumpACtrl.stateTimer > mPumpACtrl.maxOnTime)) StopOuptut(&mPumpACtrl);
  if(mPumpBCtrl.state == true && (mPumpBCtrl.stateTimer > mPumpBCtrl.maxOnTime)) StopOuptut(&mPumpBCtrl);
  if(mIrigValveCtrl.state == true && (mIrigValveCtrl.stateTimer > mIrigValveCtrl.maxOnTime)) StopOuptut(&mIrigValveCtrl);


}


void IRIG_SetupAutoIrrig(uint16_t hour1, uint16_t hour2, uint16_t duration)
{
  mIrigTime_s = 60*duration;
  mIrigHour1 = hour1;
  mIrigHour2 = hour2;
}

void IRIG_SetAutoMode(void)
{
  mAutoIrrigation = true;
}

void IRIG_IrrigateNow(uint16_t duration)
{

  mAutoIrrigation = false;
  mIrigDuration_s = duration * 60;
  StartOuptut(&mIrigValveCtrl);
}



void StartOuptut(sOutputControl* out)
{
  if((out->stateTimer > out->minOffTime) && out->state == false)
  {
    HAL_GPIO_WritePin(out->port,out->pin, GPIO_PIN_RESET);
    out->state = true;
    out->stateTimer = 0;
  }
}

void StopOuptut(sOutputControl* out)
{
  if((out->stateTimer > out->minOnTime) && out->state == true)
  {
    HAL_GPIO_WritePin(out->port,out->pin, GPIO_PIN_SET);
    out->state = false;
    out->stateTimer = 0;
  }
}



