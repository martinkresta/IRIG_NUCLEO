/*
 * RADIO.c
 *
 *  Created on: May 1, 2023
 *      Author: marti
 */


/*
 * SCOM.c
 *
 *  Created on: Jul 13, 2021
 *      Author: Martin
 *      Brief:  Universal serial communication protocol between device and computer.
 *      The handler of used UART is inserted as parameter to SCOM_Init
 */

#include "main.h"
#include "RADIO.h"
#include "VARS.h"
#include "IRIG.h"
#include "RTC.h"
#include "SONAR.h"
#include "string.h"
#include "APP.h"



uint8_t mRxBuffer[COM_BUFLEN];
uint8_t mTxBuffer[COM_BUFLEN];
uint8_t mRxLength, mNewDataReady, mTxBusy;

UART_HandleTypeDef* ComUart;

uint8_t mPcConnected;
uint16_t mPcHbTimer;
uint16_t mNsSendTimer;  // timer for sending network status


//static void SendVariable(uint16_t id);
static uint8_t Send(void);
static void ProcessMessage(void);
static void SendIrrigStatus(void);
static void SendIrrigConfig(void);
static void SendRTC(void);
static void SendUptime(void);
static void SendSonarDist(void);

void RADIO_Init(UART_HandleTypeDef* uart)
{

  ComUart = uart;
  mRxLength = 0;
  mNewDataReady = 0;
  mTxBusy = 0;
  mPcConnected = 0;

  // enable receiver
  HAL_UART_Receive_DMA(ComUart, mRxBuffer, 10);

}


void RADIO_Update_1s(void)
{

  // check PC heartbeat
  mPcHbTimer += 10;
  if (mPcHbTimer > PC_HB_TIMEOUT)
  {
    mPcConnected = 0;    // heartbeat timeout elapsed
  }
}

/* private methods */



//returns 0 when OK, 1 if transceiver is busy
static uint8_t Send(void)
{

  if (mTxBusy == 1)  // check if transciever is ready
  {
    return 1; // error: Tx Busy
  }

  mTxBusy = 1;

  HAL_UART_Transmit_DMA(ComUart, mTxBuffer, 10);

  return 0;
}


static void SendIrrigStatus(void)
{
  sIrigStatus status = IRIG_GetStatus();
 // uint16_t tmp = SONAR_GetDistance_mm();
  memset(mTxBuffer, 0, 10);
  mTxBuffer[0] = RCMD_IRIG_STATUS >> 8;
  mTxBuffer[1] = RCMD_IRIG_STATUS & 0xFF;
  memcpy(&(mTxBuffer[2]), &status, 8);
  Send();
}

static void SendIrrigConfig(void)
{
  sIrigConfig cfg = IRIG_GetConfig();
  memset(mTxBuffer, 0, 10);
  mTxBuffer[0] = RCMD_IRIG_CONFIG >> 8;
  mTxBuffer[1] = RCMD_IRIG_CONFIG & 0xFF;
  memcpy(&(mTxBuffer[2]), &cfg, 6);
  Send();
}

static void SendRTC(void)
{
  sDateTime now = RTC_GetTime();
  memset(mTxBuffer, 0, 10);
  mTxBuffer[0] = RCMD_RTC_INFO >> 8;
  mTxBuffer[1] = RCMD_RTC_INFO & 0xFF;

  mTxBuffer[2] = now.Hour;
  mTxBuffer[3] = now.Minute;
  mTxBuffer[4] = now.Second;
  mTxBuffer[5] = now.Day;
  mTxBuffer[6] = now.Month;
  mTxBuffer[7] = now.Year - 2000;
  Send();
}

static void SendUptime(void)
{
  uint32_t uptime = APP_GetUpTime();
  memset(mTxBuffer, 0, 10);
  mTxBuffer[0] = RCMD_UPTIME >> 8;
  mTxBuffer[1] = RCMD_UPTIME & 0xFF;
  memcpy(&(mTxBuffer[2]), &uptime, 4);
  Send();


}

static void SendSonarDist(void)
{
   uint16_t dist = SONAR_GetDistance_mm();
   memset(mTxBuffer, 0, 10);
   mTxBuffer[0] = RCMD_SONAR_DIST >> 8;
   mTxBuffer[1] = RCMD_SONAR_DIST & 0xFF;
   memcpy(&(mTxBuffer[2]), &dist, 2);

   Send();
}

static void ProcessMessage(void)
{
    sDateTime now;
    uint16_t id = (mRxBuffer[0]<<8) | mRxBuffer[1];

    uint16_t data1, data2, data3;// data4;
    data1 = (mRxBuffer[2]<<8) | mRxBuffer[3];
    data2 = (mRxBuffer[4]<<8) | mRxBuffer[5];
    data3 = (mRxBuffer[6]<<8) | mRxBuffer[7];
  //  data4 = (mRxBuffer[8]<<8) | mRxBuffer[9];

    switch (id )  // message ID
    {
      case RCMD_IRIG_AUTO:
        IRIG_SetAutoMode();
        break;
      case RCMD_IRIG_NOW:
        IRIG_IrrigateNow(data1);
        break;
      case RCMD_IRIG_SET_CONFIG:
        IRIG_SetupAutoIrrig(data1, data2, data3);
        break;
      case RCMD_IRIG_GET_STATUS:
        SendIrrigStatus();
        break;
      case RCMD_IRIG_GET_CONFIG:
        SendIrrigConfig();
        break;
      case RCMD_GET_RTC:
        SendRTC();
        break;
      case RCMD_GET_UPTIME:
        SendUptime();
        break;
      case RCMD_GET_SONAR_DIST:
        SendSonarDist();
        break;
      case RCMD_SW_RESET:
        NVIC_SystemReset();
        break;

      case RCMD_SET_RTC:
        now.Hour = mRxBuffer[2];
        now.Minute = mRxBuffer[3];
        now.Second = mRxBuffer[4];
        now.Day =  mRxBuffer[5];
        now.Month = mRxBuffer[6];
        now.Year = 2000 + mRxBuffer[7];
        RTC_SetTime(now);
       // RTC_SetUnixTime(unixtime);   // this fcn is too big and does not fit to flash :-(
        break;
    }

  HAL_UART_Receive_DMA(ComUart, mRxBuffer, 10);
  return;
}




/* Interrupt callbacks*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == ComUart)
  {
    mTxBusy = 0;
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == ComUart)
  {
    mNewDataReady = 1;
    mRxLength = 1;
    ProcessMessage();
  }
}
