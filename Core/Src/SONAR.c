/*
 * SONAR.c
 *
 *  Created on: Apr 29, 2023
 *      Author: marti
 */

#include "SONAR.h"
#include "main.h"


uint16_t mDistance;

void SONAR_Init(void)
{
  htim2.Instance->CR1 &= ~TIM_CR1_CEN;
  htim2.Instance->CCMR1 &= ~TIM_CCMR1_CC2S;
  htim2.Instance->CCMR1 |= TIM_ICSELECTION_DIRECTTI << TIM_CCMR1_CC2S_Pos;
  htim2.Instance->CCER = 0;
  htim2.Instance->CCMR1 |= TIM_ICSELECTION_INDIRECTTI << TIM_CCMR1_CC1S_Pos;
  htim2.Instance->CCER |= TIM_CCER_CC1P;
  htim2.Instance->SMCR = TIM_SLAVEMODE_RESET | TIM_TS_TI2FP2;

  TIM21->PSC = 31; /* (1) */
  TIM21->ARR = 14; /* (2) */
  TIM21->CCR1 = 5; /* (3) */
  TIM21->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1PE;
  return;
}

uint16_t SONAR_Measure(void)
{
  uint16_t duration_us;
  // start echo capture timer
  htim2.Instance->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
  htim2.Instance->CR1 |= TIM_CR1_CEN;


  TIM21->CCER |= TIM_CCER_CC1E; /* (5) */
  TIM21->CR1 |= TIM_CR1_OPM | TIM_CR1_ARPE; /* (6) */

  htim21.Instance->CR1 |= TIM_CR1_CEN;


  HAL_Delay(1);
  // read the echo duration
  duration_us = htim2.Instance->CCR1;


  // calculate the distance in mm
  mDistance = (duration_us*10)/58;

  return mDistance;
}

uint16_t SONAR_GetRawDistance_mm(void)
{
  return mDistance;
}

uint16_t SONAR_GetDistance_mm(void)
{
  return mDistance;
}
