/*
 * RADIO.h
 *
 *  Created on: May 1, 2023
 *      Author: marti
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_


#include "main.h"


#define RADIO_BAUDRATE 9600

#define COM_BUFLEN  10

// To IRIG
#define  RCMD_IRIG_AUTO            0x0501
#define  RCMD_IRIG_NOW             0x0502
#define  RCMD_IRIG_SET_CONFIG      0x0503

#define  RCMD_IRIG_GET_STATUS      0x0504
#define  RCMD_IRIG_GET_CONFIG      0x0505
#define  RCMD_GET_RTC              0x0506
#define  RCMD_GET_UPTIME           0x0507
#define  RCMD_GET_SONAR_DIST       0x0508
#define  RCMD_SW_RESET             0x0509

#define  RCMD_SET_RTC              0x0510

// FROM IRIG
#define  RCMD_IRIG_STATUS          0x0604
#define  RCMD_IRIG_CONFIG          0x0605
#define  RCMD_RTC_INFO             0x0606
#define  RCMD_UPTIME               0x0607
#define  RCMD_SONAR_DIST           0x0608





#define PC_HB_TIMEOUT     3000
#define NS_SEND_PERIOD    1000

void RADIO_Init(UART_HandleTypeDef* uart);
void RADIO_Update_1s(void);



#endif /* INC_RADIO_H_ */
