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


#define  RCMD_IRIG_AUTO            0x501
#define  RCMD_IRIG_NOW             0x502
#define  RCMD_IRIG_SETUP           0x503
#define  RCMD_IRIG_READ_STATUS     0x504

#define  RCMD_IRIG_STATUS          0x604

#define  RCMD_RTC_SYNC             0x510




#define PC_HB_TIMEOUT     3000
#define NS_SEND_PERIOD    1000

void RADIO_Init(UART_HandleTypeDef* uart);
void RADIO_Update_1s(void);



#endif /* INC_RADIO_H_ */
