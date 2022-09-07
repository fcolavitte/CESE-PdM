/*
 * API_delay.c
 *
 *  Created on: 7 sep. 2022
 *      Author: Facundo
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_delay.h"

void delayInit( delay_t * delay, tick_t duration ){
	delay->startTime=HAL_GetTick();
	delay->duration=duration;
	delay->running=0;//false
}

bool_t delayRead( delay_t * delay ){
	if(HAL_GetTick() - delay->startTime>delay->duration){
		delay->running=1;//true
	}
	return delay->running;
}

void delayWrite( delay_t * delay, tick_t duration ){
	delay->duration=duration;
}
