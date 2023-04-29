/*
 * SONAR.h
 *
 *  Created on: Apr 29, 2023
 *      Author: marti
 */

#ifndef INC_SONAR_H_
#define INC_SONAR_H_

#include "main.h"



#define SONAR_AVG_FILTER_SIZE


void  SONAR_Init(void);

uint16_t SONAR_Measure(void);

uint16_t SONAR_GetRawDistance_mm(void);

uint16_t SONAR_GetDistance_mm(void);



#endif /* INC_SONAR_H_ */

