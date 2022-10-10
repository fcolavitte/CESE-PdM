/*
 * API_pwm.h
 *
 *  Created on: 10 oct. 2022
 *      Author: Facu
 */

#ifndef API_INC_API_PWM_H_
#define API_INC_API_PWM_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_nucleo_144.h"
#include "stm32f429xx.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef bool bool_t;


/**
 * @brief	Configura el LED1 (Verde) como PWM dependiente del Timer3
 * @return	True si se inició correctamente. Sino regresa False
 */
bool_t pwm_init(void);

/**
 * @brief	Setea el duty cycle del PWM
 * @param	Duty cycle, de 0 a 100.
 */
void pwm_on(uint8_t dutyCycle);

#endif /* API_INC_API_PWM_H_ */
