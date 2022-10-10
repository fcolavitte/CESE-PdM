/*
 * API_num_strings.c
 *
 *  Created on: 10 oct. 2022
 *      Author: Facu
 */

#include "API_num_strings.h"

/**
 * @brief Variable para almacenar el número en formato string.
 * @note  11 posiciones para un uint32_t: 10 para el máximo valor (2^32-1)=4.294.967.295. y uno adicional para el '\0'
 */
uint8_t string_uint[11];

/**
* @brief  Función para convertir uint a string
* @param  Número a convertir
* @retval Número en forma de String (con '\0' al final)
*/
uint8_t *uint_to_string(uint32_t numero){
	uint32_t divisor;		// Divisor potencia de 10 para "desplazar la coma" en sistema decimal
	uint8_t Exp_divisor;	//Exponente del divisor, define la potencia de 10
	uint32_t digito;		//Dígito del 0 al 9 sin codificación ASCII
	uint8_t i = 0;			//Posición en el string
	bool_t  start_num = 0;	//Var aux para eliminar 0's a izquierda del dígito significativo
	for(uint8_t posicion=0 ; posicion<10 ; posicion++){
		Exp_divisor = 9 - posicion;

		/*divisor=10^Exp_divisor*/
		divisor = 1;
		for(uint8_t j=0 ; j<Exp_divisor ; j++){
			divisor = divisor * 10;
		}

		digito = (numero/divisor)%10;
		if(digito!=0||start_num!=0){
			string_uint[i++] = digito + '0';	//Agregar dígito al String en código ASCII
			start_num = 1;
		}
	}

	//Si el número original fue 0 definir string como "0", sino devuelve '\0'
	if(string_uint[0]=='\0'){
		string_uint[0]='0';
		i=1;
	}

	//Cerrar string
	string_uint[i] = '\0';
	return string_uint;
}
