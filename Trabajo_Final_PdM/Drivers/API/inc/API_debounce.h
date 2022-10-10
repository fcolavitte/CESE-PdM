/*
 * API_debounce.h
 *
 *  Created on: 13 sep. 2022
 *      Author: Facundo
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

//--------- includes & defines -------------------------------------------------------
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h" 	/* BSP include */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "API_delay.h"

#define T_ANTIREBOTE 40	//Tiempo de anti-rebote en ms

//--------- Functions ----------------------------------------------------------------
/**
 * @brief	Inicializa la máquina de estados del pulsador y la estructura
 * 			delay para el anti-rebote
 */
void debounceFSM_init(void);

/**
 * @brief	Actualiza estado del pulsador.
 */
void debounceFSM_update(void);

/**
 * @brief	Verifica si se precionó el pulsador
 * @return	True si se precionó desde la última consulta, sino False
 */
bool_t readKey(void);

/**
 * @brief	Verifica si hubo un flanco ascendente
 * @return	True si hubo, sino false
 */
bool_t readFlancoAscendente(void);

/**
 * @brief	Verifica si hubo un flanco descendente
 * @return	True si hubo, sino false
 */
bool_t readFlancoDescendente(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
