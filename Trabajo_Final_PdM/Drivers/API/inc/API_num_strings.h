/*
 * API_num_strings.h
 *
 *  Created on: 10 oct. 2022
 *      Author: Facu
 */

#ifndef API_INC_API_NUM_STRINGS_H_
#define API_INC_API_NUM_STRINGS_H_

#include <stdint.h>
#include <stdbool.h>


typedef bool bool_t;


/**
* @brief  Función para convertir uint a string
* @param  Número a convertir
* @retval Número en forma de String (con '\0' al final)
*/
uint8_t *uint_to_string(uint32_t numero);


#endif /* API_INC_API_NUM_STRINGS_H_ */
