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
#include "UI.h"
#include "SONAR.h"


sOutputControl mPumpACtrl;
sOutputControl mPumpBCtrl;
sOutputControl mIrigValveCtrl;


sIrigStatus mState;

sIrigConfig mConfig;

bool mAutoIrrigation;
bool mFlushTank;
bool mTankFull;
bool mWellEmpty;
// irigation settings

uint16_t mIrigTime_s;
uint16_t mIrigHour1;
uint16_t mIrigHour2;

uint16_t mIrigDuration_s;

//uint16_t mPumpADailyOnTime_s;
//uint16_t mDailyIrrigLiters_l;

uint16_t mIrigStartLiters;
uint16_t mIrigStopLiters;

//uint16_t mTankLevel_l;    // litres available in tank



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
  mIrigValveCtrl.minOffTime = 0;  // 1 seconds
  mIrigValveCtrl.minOnTime = 0;
  mIrigValveCtrl.maxOnTime = 4* 60 *60;  // 4 hours
  mIrigValveCtrl.pin = REL3_Pin;
  mIrigValveCtrl.port = REL3_GPIO_Port;
  mIrigValveCtrl.stateTimer = 60000;


  mIrigTime_s = 2*60*60;  // 2 hour
  mIrigHour1 = 7;       // morning 7:00
  mIrigHour2 = 19;      // evening 7:00  // hint: to disable one irrigation period, set the hour to more than 24 ;-)

  mConfig.irigDuration_s = mIrigTime_s;
  mConfig.irigHour1 = mIrigHour1;
  mConfig.irigHour2 = mIrigHour2;

  mAutoIrrigation = true;
  mFlushTank = false;
}


void IRIG_Midnight(void)
{
  mState.dailyIrrigLiters_l = 0;
  mState.pumpADailyOnTime_s = 0;
}

void IRIG_Update_1s(void)
{
  sDateTime now = RTC_GetTime();

  mState.tankLevel_l = ((860 - (SONAR_GetDistance_mm())) * 3) / 10;

  // read the level bi-state sensors

  mTankFull = DI_Get(IN1_LEVEL1_TANKFULL); // water level above sensor
 // tankEmpty = !DI_Get(IN2_LEVEL2_TANKEMPTY);  // water level below sensor
  mWellEmpty = !DI_Get(IN3_LEVEL3_WELLAEMPTY);  // water level below sensor


  // automatic irrigation

  if(mAutoIrrigation == true)
  {
    if (now.Hour == mIrigHour1 || now.Hour == mIrigHour2)
      {
        if(now.Minute == 00 && now.Second == 0)
        {
          mIrigDuration_s = mIrigTime_s;
          mIrigStartLiters = mState.tankLevel_l;
          StartOuptut(&mIrigValveCtrl);
        }
      }
  }


  // automatic refilling of irrigation tank from well (gray water savage)
  if(!mFlushTank)   // disable refilling if user wants to flush the tank
  {
    if(!(mAutoIrrigation == true && mIrigValveCtrl.state == true))
    {
      if(!mTankFull && !mWellEmpty)
      {
        // limit the pump start to every quarter past, to spread power supply load in time.
        // (The other savage pump is running always from XX:00 to XX:07)
        //if(now.Minute == 15)
        //{
        //  StartOuptut(&mPumpACtrl);
        //}
        StartOuptut(&mPumpACtrl);
      }
    }
  }
  // stopping the refilling pump
  if(mTankFull || mWellEmpty)
  {
    StopOuptut(&mPumpACtrl);
  }


  if(!mFlushTank)
  {
    // stoping of irrigation (both manual and auto triggered)
     if(mIrigValveCtrl.state == true && (mIrigValveCtrl.stateTimer > mIrigDuration_s))
     {
       StopOuptut(&mIrigValveCtrl);
       mState.dailyIrrigLiters_l += mIrigStartLiters - mState.tankLevel_l;
     }

     // apply output time limitations
     // increment the timers preventing overflow (maximal timing is 1000 minutes)
     if(mPumpACtrl.stateTimer < 60000) mPumpACtrl.stateTimer++;
     if(mPumpBCtrl.stateTimer < 60000) mPumpBCtrl.stateTimer++;
     if(mIrigValveCtrl.stateTimer < 60000) mIrigValveCtrl.stateTimer++;

     // check max ON time of all inputs
     if(mPumpACtrl.state == true && (mPumpACtrl.stateTimer > mPumpACtrl.maxOnTime)) StopOuptut(&mPumpACtrl);
     if(mPumpBCtrl.state == true && (mPumpBCtrl.stateTimer > mPumpBCtrl.maxOnTime)) StopOuptut(&mPumpBCtrl);
     if(mIrigValveCtrl.state == true && (mIrigValveCtrl.stateTimer > mIrigValveCtrl.maxOnTime))  StopOuptut(&mIrigValveCtrl);
  }

  //------------------------
  // UI update
  // update Green LED - Irrigation state
  if(mIrigValveCtrl.state == true)  UI_LED_G_SetMode(eUI_ON);
  else                              UI_LED_G_SetMode(eUI_FLASH);
  // update RED LED - Pump
  if(mPumpACtrl.state == true)
  {
    UI_LED_R_SetMode(eUI_ON);
    mState.pumpADailyOnTime_s ++;   // increment pump On Timer
  }
  else if(!mTankFull && mWellEmpty) UI_LED_R_SetMode(eUI_BLINKING_FAST);
  else                                UI_LED_R_SetMode(eUI_OFF);

}


void IRIG_SetupAutoIrrig(uint16_t hour1, uint16_t hour2, uint16_t duration)
{
  mIrigTime_s = 60*duration;
  mIrigHour1 = hour1;
  mIrigHour2 = hour2;

  mConfig.irigDuration_s = mIrigTime_s;
  mConfig.irigHour1 = mIrigHour1;
  mConfig.irigHour2 = mIrigHour2;
}

sIrigStatus IRIG_GetStatus(void)
{
  mState.statusFlags = 0;
  mState.statusFlags |= (mIrigValveCtrl.state ? 1 : 0) << IRIG_STATFLAG_IRIGVALVE;
  mState.statusFlags |= (mPumpACtrl.state ? 1 : 0) << IRIG_STATFLAG_PUMP_A;
  mState.statusFlags |= (mPumpBCtrl.state ? 1 : 0) << IRIG_STATFLAG_PUMP_B;
  mState.statusFlags |= (mAutoIrrigation ? 1 : 0) << IRIG_STATFLAG_AUTOIRRIG;
  mState.statusFlags |= (mFlushTank ? 1 : 0) << IRIG_STATFLAG_FLUSHTANK;
  mState.statusFlags |= (mTankFull ? 1 : 0) << IRIG_STATFLAG_TANKFULL;
  mState.statusFlags |= (mWellEmpty ? 1 : 0) << IRIG_STATFLAG_WELLAEMPTY;
  return mState;
}

sIrigConfig IRIG_GetConfig(void)
{
  return mConfig;
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

void IRIG_ToggleIrrigiation(void)
{
  if(mIrigValveCtrl.state == false)
  {
    mIrigDuration_s = 60 * 180;   // max 3 hours
    StartOuptut(&mIrigValveCtrl);
  }
  else
  {
    StopOuptut(&mIrigValveCtrl);
  }
}

void IRIG_FlushTank(void)
{
  mFlushTank = true;
  StartOuptut(&mIrigValveCtrl);
  StopOuptut(&mPumpACtrl);
}

void IRIG_FillTank(void)
{
  mFlushTank = false;
  StopOuptut(&mIrigValveCtrl);
}



void StartOuptut(sOutputControl* out)
{
  if(out->state == true) return;
  if((out->stateTimer > out->minOffTime) && out->state == false)
  {
    HAL_GPIO_WritePin(out->port,out->pin, GPIO_PIN_RESET);
    out->state = true;
    out->stateTimer = 0;
  }
}

void StopOuptut(sOutputControl* out)
{
  if(out->state == false) return;
  if((out->stateTimer > out->minOnTime) && out->state == true)
  {
    HAL_GPIO_WritePin(out->port,out->pin, GPIO_PIN_SET);
    out->state = false;
    out->stateTimer = 0;
  }
}



