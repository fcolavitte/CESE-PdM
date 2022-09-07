/*
 * API_delay.h
 *
 *  Created on: 7 sep. 2022
 *      Author: Facundo
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_


#include <stdbool.h>
typedef uint32_t tick_t;
typedef bool bool_t;	  // stdbool.h
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );

#endif /* API_INC_API_DELAY_H_ */
