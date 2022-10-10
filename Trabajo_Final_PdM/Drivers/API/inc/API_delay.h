/*
 * API_delay.h
 *
 *  Created on: 7 sep. 2022
 *      Author: Facundo
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

//--------- includes & defines -----------
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_DELAY 30000	//30 segundos

//--------- typedefs ---------------------
typedef uint32_t tick_t;  // stdint.h

typedef bool bool_t;	  // stdbool.h

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

//--------- Functions ---------------------
/**
 * @brief	Inicializa una estructura delay
 * @param	Puntero a estructura delay
 * @param	Duración del retardo no bloqueante en ms
 * @return	None
 */
void delayInit( delay_t * delay, tick_t duration );

/**
 * @brief	Actualización y lectura del estado del delay. Reinicia el mismo si este ya finalizó
 * @param	Puntero a estructura delay
 * @return	True si terminó retardo, sino False
 */
bool_t delayRead( delay_t * delay );

/**
 * @brief	Cambio del valor de retardo del delay
 * @param	Puntero a estructura delay
 * @param	Nueva duración del retardo no bloqueante en ms
 * @return	None
 */
void delayWrite( delay_t * delay, tick_t duration );

#endif /* API_INC_API_DELAY_H_ */
