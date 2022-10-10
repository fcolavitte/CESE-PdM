/*
 * API_uart.c
 *
 *  Created on: 21 sep. 2022
 *      Author: Facu
 */

/* Includes ------------------------------------------------------------------*/
#include "API_uart.h"


// UART handler declaration
static UART_HandleTypeDef UartHandle;



/**
* @brief  Inicializa la UART. Envía parametros de inicialización por UART
* @param  None
* @retval true:UART se inicializó correctamente, false:falla al inicializar UART
*/
bool_t uartInit(void){

	UartHandle.Instance        = USARTx;

	UartHandle.Init.BaudRate   		= BAUDRATE;
	UartHandle.Init.WordLength 		= WORD_LENGTH;
	UartHandle.Init.StopBits   		= STOP_BITS;
	UartHandle.Init.Parity     		= PARITY;
	UartHandle.Init.HwFlowCtl  		= HW_FLOW_CTL;
	UartHandle.Init.Mode       		= UART_MODE_TX_RX;
	UartHandle.Init.OverSampling 	= 16;

	/*Inicializar UART --------------------------------------------*/
	if (HAL_UART_Init(&UartHandle) != HAL_OK) {
	  return 0;
	}


	/*Enviar parámetros de configuración por UART -----------------*/
	uartSendString((uint8_t *)"\r\n>> UART inicializado correctamente\r\n");
	uartSendString((uint8_t *)">> Parametros de configuracion UART:\r\n");
	uartSendString((uint8_t *)"    > BAUDRATE:  ");
	uartSendString(uint_to_string(BAUDRATE));
	uartSendString((uint8_t *)"\r\n    > WORD_LENGTH:  ");
	if(WORD_LENGTH==UART_WORDLENGTH_8B){
		uartSendString((uint8_t *)"8bits\r\n");
	}else{
		uartSendString((uint8_t *)"9bits\r\n");
	}
	uartSendString((uint8_t *)"    > STOP_BITS:  ");
	if(STOP_BITS==UART_STOPBITS_1){
		uartSendString((uint8_t *)"1\r\n");
	}else{
		uartSendString((uint8_t *)"2\r\n");
	}
	uartSendString((uint8_t *)"    > PARITY:  ");
	if(PARITY==UART_PARITY_ODD){
		uartSendString((uint8_t *)"ODD\r\n");
	}else if(PARITY==UART_PARITY_EVEN){
		uartSendString((uint8_t *)"EVEN\r\n");
	}else {
		uartSendString((uint8_t *)"NONE\r\n");
	}
	uartSendString((uint8_t *)"    > HW_FLOW_CTL:  ");
	switch(HW_FLOW_CTL){
		case UART_HWCONTROL_NONE:
			uartSendString((uint8_t *)"NONE\r\n");
		break;
		case UART_HWCONTROL_RTS:
			uartSendString((uint8_t *)"RTS\r\n");
		break;
		case UART_HWCONTROL_CTS:
			uartSendString((uint8_t *)"CTS\r\n");
		break;
		case UART_HWCONTROL_RTS_CTS:
			uartSendString((uint8_t *)"RTS-CTS\r\n");
		break;
		default:
			uartSendString((uint8_t *)"Error en reconocer HW_Control");
		break;
	}
	uartSendString((uint8_t *)"    > UART_MODE:  UART_MODE_TX_RX\r\n");
	uartSendString((uint8_t *)"    > OVER_SAMPLING:  16\r\n");

	return 1;
}


/**
* @brief  Envía un String por UART. Longitud máxima del array definida por UART_LONG_MAX (incluyendo el '\0')
* @param  Puntero a cadena de caracteres a enviar
* @retval None
*/
void uartSendString(uint8_t * pstring){
	if(pstring!=0){
		uint16_t size=0;
		bool_t is_string=0;
		for(int i=0;i<UART_LONG_MAX;i++){
			if(pstring[i]=='\0'){
				is_string=1;
				size=i;
				i=UART_LONG_MAX;
			}
		}
		if(is_string==0){	//Convertir en String y recortar
			pstring[UART_LONG_MAX-1]='\0';
			size=UART_LONG_MAX-1;
		}

		uartSendStringSize(pstring, size);
	} else {
		/* Control de Error*/
	}
}


/**
* @brief  Envía String por UART.
* @param  puntero al string a enviar
* @param  cantidad de caracteres a enviar
* @retval None
*/
void uartSendStringSize(uint8_t * pstring, uint16_t size){
	if(pstring!=0 && size>0){
		HAL_UART_Transmit(&UartHandle, pstring, size, 0xffff);
	} else {
		/*Control de Errror*/
	}
}



/**
* @brief  Lee puerto UART
* @param  puntero al string donde se recibirá la lectura
* @param  cantidad de caracteres a recibir
* @retval None
*/
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	if(pstring!=0 && size>0){
		if(size==1){
			HAL_UART_Receive(&UartHandle, pstring, size, 0x0002);
		}else{
			HAL_UART_Receive(&UartHandle, pstring, size, 0x000f);
		}
	} else {
		/*Control de Error*/
	}
}

