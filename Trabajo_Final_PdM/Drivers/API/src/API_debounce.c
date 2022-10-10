/*
 * API_debounce.c
 *
 *  Created on: 13 sep. 2022
 *      Author: Facundo
 */

#include "API_delay.h"
#include "API_debounce.h"

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;


//------ Private variables --------------
static debounceState_t stateButton;
static delay_t delay;
static bool_t bool_buttonPressed=0;
static bool_t bool_buttonRised=0;
static bool_t bool_buttonFalled=0;


/**
 * @brief	Inicializa la máquina de estados del pulsador y la estructura
 * 			delay para el anti-rebote
 * @param	None
 * @return	None
 */
void debounceFSM_init(void){
	stateButton=BUTTON_UP;
	delayInit(&delay,T_ANTIREBOTE);
}


/**
 * @brief	Actualiza estado del pulsador.
 * @param	None
 * @return	None
 */
void debounceFSM_update(void){
	switch (stateButton){
		case BUTTON_UP:
			if(BSP_PB_GetState(BUTTON_USER)){
				stateButton=BUTTON_FALLING;
			}
		break;
		case BUTTON_FALLING:
			if(delayRead(&delay)){
				if(BSP_PB_GetState(BUTTON_USER)){
					stateButton=BUTTON_DOWN;
					bool_buttonPressed=1;
					bool_buttonFalled=1;
				}else{
					stateButton=BUTTON_UP;
				}
			}
		break;
		case BUTTON_DOWN:
			if(!BSP_PB_GetState(BUTTON_USER)){
				stateButton=BUTTON_RAISING;
			}
		break;
		case BUTTON_RAISING:
			if(delayRead(&delay)){
				if(BSP_PB_GetState(BUTTON_USER)){
					stateButton=BUTTON_DOWN;
				}else{
					stateButton=BUTTON_UP;
					bool_buttonRised=1;
				}
			}
		break;
		default:
			//>control de error<
			debounceFSM_init();
		break;
	}
}


/**
 * @brief	Verifica si se precionó el pulsador
 * @param	None
 * @return	True si se precionó desde la última consulta, sino False
 */
bool_t readKey(void){
	if(bool_buttonPressed){
		bool_buttonPressed=0;
		return 1;
	} else {
		return 0;
	}
}


/**
 * @brief	Verifica si hubo un flanco ascendente
 * @param	None
 * @return	True si hubo, sino false
 */
bool_t readFlancoAscendente(void){
	if(bool_buttonRised){
		bool_buttonRised=0;
		return 1;
	} else {
		return 0;
	}
}


/**
 * @brief	Verifica si hubo un flanco descendente
 * @param	None
 * @return	True si hubo, sino false
 */
bool_t readFlancoDescendente(void){
	if(bool_buttonFalled){
		bool_buttonFalled=0;
		return 1;
	} else {
		return 0;
	}
}
