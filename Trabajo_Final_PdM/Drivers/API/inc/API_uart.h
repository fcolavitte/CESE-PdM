/*
 * API_uart.h
 *
 *  Created on: 21 sep. 2022
 *      Author: Facu
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "API_num_strings.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"


#define UART_LONG_MAX 100	/*Longitud máxima de cadena de caracteres a enviar por UART*/

/* ----------- Parámetros para comunicación UART -----------------------------------------------*/
static const uint32_t BAUDRATE		= 9600;
static const uint32_t WORD_LENGTH	= UART_WORDLENGTH_8B;
static const uint32_t STOP_BITS		= UART_STOPBITS_1;
static const uint32_t PARITY		= UART_PARITY_ODD;
static const uint32_t HW_FLOW_CTL	= UART_HWCONTROL_NONE;



typedef bool bool_t;	  // stdbool.h


/*------------------------ Funciones -----------------------------------------------------------*/
/**
* @brief  Inicializa la UART. Envía parametros de inicialización por UART
* @param  None
* @retval true:UART se inicializó correctamente, false:falla al inicializar UART
*/
bool_t uartInit(void);

/**
* @brief  Envía un String por UART. Longitud máxima del array definida por UART_LONG_MAX (incluyendo el '\0')
* @param  Puntero a cadena de caracteres a enviar
* @retval None
*/
void uartSendString(uint8_t * pstring);

/**
* @brief  Envía String por UART.
* @param  puntero al string a enviar
* @param  cantidad de caracteres a enviar
* @retval None
*/
void uartSendStringSize(uint8_t * pstring, uint16_t size);

/**
* @brief  Lee puerto UART
* @param  puntero al string donde se recibirá la lectura
* @param  cantidad de caracteres a recibir
* @retval None
*/
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
