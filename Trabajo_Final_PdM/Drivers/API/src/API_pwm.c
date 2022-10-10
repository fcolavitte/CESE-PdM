/*
 * API_pwm.c
 *
 *  Created on: 10 oct. 2022
 *      Author: Facu
 */

#include "API_pwm.h"


static TIM_HandleTypeDef hTim3;

/**
 * @brief	Configura el LED1 (Verde) como PWM dependiente del Timer3
 * @return	True si se inició correctamente. Sino regresa False
 */
bool_t pwm_init(void){
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  // Configuración del pin LED1 como PWM dependientel del Timer 3
	  GPIO_InitStruct.Pin = GPIO_PIN_0;				//PB0 -> Timer_3 channel_3
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;

	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);		//PB0 -> Timer_3 channel_3

	  //Configuración Timer3
	  __HAL_RCC_TIM3_CLK_ENABLE();
	  hTim3.Instance = TIM3;
	  hTim3.Init.Prescaler = 80-1;
	  hTim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  hTim3.Init.Period = 1000-1;
	  hTim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  hTim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if(HAL_TIM_Base_Init(&hTim3) != HAL_OK){
		  return false;
	  }

	  //Selección del reloj interno
	  TIM_ClockConfigTypeDef TimClock = {0};
	  TimClock.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if(HAL_TIM_ConfigClockSource(&hTim3, &TimClock) != HAL_OK){
		  return false;
	  }

	  if(HAL_TIM_PWM_Init(&hTim3) != HAL_OK){
		  return false;
	  }

	  //Configuración del Time Output Compare, para generar salida PWM
	  TIM_OC_InitTypeDef TimOC = {0};
	  TimOC.OCMode = TIM_OCMODE_PWM1;
	  TimOC.Pulse = 0;
	  TimOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  TimOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if(HAL_TIM_PWM_ConfigChannel(&hTim3, &TimOC, TIM_CHANNEL_3) != HAL_OK){
		  return false;
	  }

	  HAL_TIM_PWM_Start(&hTim3, TIM_CHANNEL_3);
	  __HAL_TIM_ENABLE(&hTim3);
	  return true;
}


/**
 * @brief	Setea el duty cycle del PWM
 * @param	Duty cycle, de 0 a 100.
 */
void pwm_on(uint8_t dutyCycle){
	uint16_t ccr_value;
	if(dutyCycle>100){dutyCycle=100;}

	//Multiplicar el dutyCycle y el periodo del timer para definir el nuevo valor del CCR para señal PWM
	ccr_value = (uint16_t) ((__HAL_TIM_GET_AUTORELOAD(&hTim3)+0.0f)*(dutyCycle/100.0f));
	__HAL_TIM_SET_COMPARE(&hTim3, TIM_CHANNEL_3, ccr_value);
}

