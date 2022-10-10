/**
 * @file API_dht22.c
 * @version v1.0
 * @date 05/10/2022
 * @author Facundo Colavitte
 */


#include "API_dht22.h"


/*DHT22 Functions --------------------------------------------------------------------------*/
static void decodificar(void);
static bool_t comrpobar_datos(void);

/*Functions GPIO ---------------------------------------------------------------------------*/
static void GPIO_set_config(uint8_t GPIO_port, uint16_t GPIO_num);
static void GPIO_write(uint8_t GPIO_port, uint16_t GPIO_num, bool_t GPIO_state);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
static void EXTI9_5_IRQHandler(void);
static GPIO_TypeDef  * _HAL_PORT_DECODE(uint8_t Port);

/*Functions time & timer -------------------------------------------------------------------*/
static void Timer_Init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);



/*Variables --------------------------------------------------------------------------------*/
/** Handler para Timer2											*/
static TIM_HandleTypeDef hTim2;

/** Contador para definir la posición de bits de la comunicación*/
static uint8_t T_Array_counter = 0;

/** Cuenta los ms con interrupciones del Timer2					*/
static uint32_t cont_timer=0;



/*Variables Globales -----------------------------------------------------------------------*/

/** Variable en la que se almacenan los parámetros y lecturas del DHT22	*/
static DHT22_sensor _DHT22;


/**
 * Variable donde se guardan los bits en el proceso de lectura durante las interrupciones. No se
 * guardan directamente en _DHT22.data.crude para no modificar el valor mientras se lo decodifica
 */
static uint32_t _data_crude;

/**
 * Variable donde se guardan los bits en el proceso de lectura durante las interrupciones. No se
 * guardan directamente en _DHT22.data.validation para no modificar el valor mientras se lo decodifica
 */
static uint8_t _data_validation;



/**
 * @brief	Inicializa las funciones de lectura del DHT22 en el GPIO asignado
 * @param	Número de GPIO del Port. Revisar HAL o datsheet para obtener el número de cada pin.
 * @param	Valores válidos PORT_A a PORT_G
 * @return	True si se inició correctamente. Sino regresa False
 */
bool_t DHT22_init(uint16_t _pin, uint8_t _port){
	if(IS_GPIO_PIN(_pin)){

		_DHT22.Pin=_pin;
		_DHT22.Port=_port;

		//Cargar valores por defecto
		_DHT22.data.temp = 0;
		_DHT22.data.temp_string[0] = '\0';
		_DHT22.data.hum  = 0;
		_DHT22.data.hum_string[0]  = '\0';
		_DHT22.data.crude = 0;
		_DHT22.data.validation = 0;
		_DHT22.time_last_call = 0;
		_DHT22.status = DHT_CONNECTING;

		GPIO_set_config(_DHT22.Port, _DHT22.Pin);

		GPIO_write(_DHT22.Port, _DHT22.Pin, 1);

		return 1;
	}
	return 0;
}


/**
 * @brief	Solicita el valor de temperatura
 * @return	Temperatura en grados celcius. En caso de error devuelve valores inferiores a -900.
 * @note 	-911 ERR_COMMUNICATION, -992 DISCONECTED, -993 Tomando primer lectura, -994 CONNECTING, -999 Otros.
 */
float DHT22_get_temp(void){
	if(_DHT22.status==DHT_READED){
		_DHT22.data.crude=_data_crude;
		_DHT22.data.validation=_data_validation;
		decodificar();
	}
	switch (_DHT22.status){
		case DHT_OK:
			;
			break;
		case ERR_COMMUNICATION:
			_DHT22.data.temp = -991;
			break;
		case ERR_DISCONECT:
			_DHT22.data.temp = -992;
			break;
		case DHT_READING:
			{if(_DHT22.data.validation==0){
				_DHT22.data.temp = -993;	/*Obteniendo primera lectura*/
			}}
			break;
		case DHT_CONNECTING:
			_DHT22.data.temp = -994;		/*Todavía no se realizó primer lectura*/
			break;
		default:
			_DHT22.data.temp = -999;
			break;
	}
	return _DHT22.data.temp;
}


/**
 * @brief	Solicita el valor de temperatura
 * @return	Temperatura en grados celcius en formato String
 * @note 	E1 ERR_COMMUNICATION, E2 DISCONECTED, E3 Tomando primer lectura, E4 CONNECTING, E9 Otros.
 */
uint8_t * DHT22_get_temp_string(void){
	if(_DHT22.status==DHT_READED){
		_DHT22.data.crude=_data_crude;
		_DHT22.data.validation=_data_validation;
		decodificar();
	}
	switch (_DHT22.status){
		case DHT_OK:
			;
			break;
		case ERR_COMMUNICATION:
			_DHT22.data.temp_string[0] = 'E';
			_DHT22.data.temp_string[1] = '1';
			_DHT22.data.temp_string[2] = '\0';
			break;
		case ERR_DISCONECT:
			_DHT22.data.temp_string[0] = 'E';
			_DHT22.data.temp_string[1] = '2';
			_DHT22.data.temp_string[2] = '\0';
			break;
		case DHT_READING:
			{if(_DHT22.data.validation==0){
				_DHT22.data.temp_string[0] = 'E';
				_DHT22.data.temp_string[1] = '3';
				_DHT22.data.temp_string[2] = '\0';	/*Obteniendo primera lectura*/
			}}
			break;
		case DHT_CONNECTING:
			_DHT22.data.temp_string[0] = 'E';
			_DHT22.data.temp_string[1] = '4';
			_DHT22.data.temp_string[2] = '\0';		/*Todavía no se realizó primer lectura*/
			break;
		default:
			_DHT22.data.temp_string[0] = 'E';
			_DHT22.data.temp_string[1] = '9';
			_DHT22.data.temp_string[2] = '\0';
			break;
	}
	return _DHT22.data.temp_string;
}


/**
 * @brief	Solicita el valor de humedad
 * @return	Humedad. En caso de error devuelve valores inferiores a -900.
 * @note 	-911 ERR_COMMUNICATION, -992 DISCONECTED, -993 Tomando primer lectura, -994 CONNECTING, -999 Otros.
 */
float DHT22_get_hum(void){
	if(_DHT22.status==DHT_READED){
		_DHT22.data.crude=_data_crude;
		_DHT22.data.validation=_data_validation;
		decodificar();
	}
	switch (_DHT22.status){
		case DHT_OK:
			;
			break;
		case ERR_COMMUNICATION:
			_DHT22.data.hum = -991;
			break;
		case ERR_DISCONECT:
			_DHT22.data.hum = -992;
			break;
		case DHT_READING:
			{if(_DHT22.data.validation==0){
				_DHT22.data.hum = -993;	/*Obteniendo primera lectura*/
			}}
			break;
		case DHT_CONNECTING:
			_DHT22.data.hum = -994;		/*Todavía no se realizó primer lectura*/
			break;
		default:
			_DHT22.data.hum = -999;
			break;
	}
	return _DHT22.data.hum;
}


/**
 * @brief	Solicita el valor de humedad
 * @return	Humedad en formato String
 * @note 	E1 ERR_COMMUNICATION, E2 DISCONECTED, E3 Tomando primer lectura, E4 CONNECTING, E9 Otros.
 */
uint8_t * DHT22_get_hum_string(void){
	if(_DHT22.status==DHT_READED){
		_DHT22.data.crude=_data_crude;
		_DHT22.data.validation=_data_validation;
		decodificar();
	}
	switch (_DHT22.status){
		case DHT_OK:
			;
			break;
		case ERR_COMMUNICATION:
			_DHT22.data.hum_string[0] = 'E';
			_DHT22.data.hum_string[1] = '1';
			_DHT22.data.hum_string[2] = '\0';
			break;
		case ERR_DISCONECT:
			_DHT22.data.hum_string[0] = 'E';
			_DHT22.data.hum_string[1] = '2';
			_DHT22.data.hum_string[2] = '\0';
			break;
		case DHT_READING:
			{if(_DHT22.data.validation==0){
				_DHT22.data.hum_string[0] = 'E';
				_DHT22.data.hum_string[1] = '3';
				_DHT22.data.hum_string[2] = '\0';	/*Obteniendo primera lectura*/
			}}
			break;
		case DHT_CONNECTING:
			_DHT22.data.hum_string[0] = 'E';
			_DHT22.data.hum_string[1] = '4';
			_DHT22.data.hum_string[2] = '\0';		/*Todavía no se realizó primer lectura*/
			break;
		default:
			_DHT22.data.hum_string[0] = 'E';
			_DHT22.data.hum_string[1] = '9';
			_DHT22.data.hum_string[2] = '\0';
			break;
	}
	return _DHT22.data.hum_string;
}


/**
 * @brief	Traduce los valores guardados en _DHT22.data.crude
 */
static void decodificar(void){
	if(comrpobar_datos()){

		//La humedad son los primeros 2 bytes de la comunicación, con punto flotante con un decimal
		float humidity = (float)(_DHT22.data.crude/(1<<16));
		humidity = humidity/10;

		//La temperatura son los últimos 2 bytes de la comunicación, con punto flotante con un decimal
		float temperature = (float)(_DHT22.data.crude%(1<<15));
		temperature = temperature/10;

		//Actualizar valores numéricos en la estructura _DHT22
		_DHT22.data.hum = humidity;
		_DHT22.data.temp = temperature;

		//Actualizar valores tipo String en la estructura _DHT22
		uint8_t * data_string = uint_to_string((uint32_t)(humidity*10));
		if(humidity<10){
			_DHT22.data.hum_string[0]=data_string[0];
			_DHT22.data.hum_string[1]=',';
			_DHT22.data.hum_string[2]=data_string[1];
			_DHT22.data.hum_string[3]='%';
			_DHT22.data.hum_string[4]='\0';
		} else {
			_DHT22.data.hum_string[0]=data_string[0];
			_DHT22.data.hum_string[1]=data_string[1];
			_DHT22.data.hum_string[2]=',';
			_DHT22.data.hum_string[3]=data_string[2];
			_DHT22.data.hum_string[4]='%';
			_DHT22.data.hum_string[5]='\0';
		}

		data_string = uint_to_string((uint32_t)(temperature*10));
		if(temperature<10){
			_DHT22.data.temp_string[0]=data_string[0];
			_DHT22.data.temp_string[1]=',';
			_DHT22.data.temp_string[2]=data_string[1];
			_DHT22.data.temp_string[3]='C';	/*No toma el '°'*/
			_DHT22.data.temp_string[4]='\0';
		} else {
			_DHT22.data.temp_string[0]=data_string[0];
			_DHT22.data.temp_string[1]=data_string[1];
			_DHT22.data.temp_string[2]=',';
			_DHT22.data.temp_string[3]=data_string[2];
			_DHT22.data.temp_string[4]='C';	/*No toma el '°'*/
			_DHT22.data.temp_string[5]='\0';
		}

		//Si el primer bit de los 2bytes de temp es 1, la temperatura es negativa
		if((_DHT22.data.crude&0x8000) > 0){
			temperature=-temperature;
			for(int i=0;i<6;i++){
				_DHT22.data.temp_string[i+1]=_DHT22.data.temp_string[i];
			}
			_DHT22.data.temp_string[0]='-';
		}

	} else {	//Si hubo error en la comunicación:
		_DHT22.data.hum=-999;
		_DHT22.data.temp=-999;
		_DHT22.data.hum_string[0] = 'E';
		_DHT22.data.hum_string[1] = '\0';
		_DHT22.data.temp_string[0] = 'E';
		_DHT22.data.temp_string[1] = '\0';
	}
}


/**
 * @brief	Verifica los bits recibidos en comparación con el byte de paridad. Modifica _DHT22.status
 * @return	True si byte de paridad coincide, sino False.
 */
static bool_t comrpobar_datos(void){

	//Guardar en suma_bytes la suma de los 4 bytes que conforman el uint32_t "_DHT22.data.crude"
	uint32_t suma_bytes = _DHT22.data.crude & 0xff;
	suma_bytes += (_DHT22.data.crude>>(8*1)) & 0xff;
	suma_bytes += (_DHT22.data.crude>>(8*2)) & 0xff;
	suma_bytes += (_DHT22.data.crude>>(8*3)) & 0xff;

	//Verificar si la suma de los 4bytes del mensaje es igual al byte de paridad "_DHT22.data.validation"
	if((uint8_t)suma_bytes==_DHT22.data.validation){
		_DHT22.status = DHT_OK;
		return 1;
	} else {
		_DHT22.status = ERR_COMMUNICATION;
		/*Control de Error*/
		return 0;
	}
}




/*----------------------------------------------- GPIO -------------------------------------------------------*/


/**
 * @brief	Convierte variable PORT definida en DHT22.h a variable utilizada por la HAL para dicho puerto
 * @param	PORT_A a PORT_G
 * @param	Puntero a estructura PORT
 */
static GPIO_TypeDef  * _HAL_PORT_DECODE(uint8_t Port){
	GPIO_TypeDef  * _HAL_PORT;
	switch (Port){
		case PORT_A:
			_HAL_PORT = GPIOA;
		break;
		case PORT_B:
			_HAL_PORT = GPIOB;
		break;
		case PORT_C:
			_HAL_PORT = GPIOC;
		break;
		case PORT_D:
			_HAL_PORT = GPIOD;
		break;
		case PORT_E:
			_HAL_PORT = GPIOE;
		break;
		case PORT_F:
			_HAL_PORT = GPIOF;
		break;
		default:
			_HAL_PORT = GPIOG;
		break;
	}
	return _HAL_PORT;
}


/**
 * @brief	Configurar GPIO para comunicación con DHT22
 * @param	PORT_A a PORTG
 * @param	Pin, en caso de STM32 va de GPIO_PIN_0 a GPIO_PIN_15
 * @note	GPIO modo salida, open-drain con interrupciones por flanco descendente
 */
static void GPIO_set_config(uint8_t GPIO_port, uint16_t GPIO_num){
	if(IS_GPIO_PIN(GPIO_num)){
		//Preparar configuración PIN
		GPIO_InitTypeDef PIN_DHT22_config;

		if(PORT_A == GPIO_port){__HAL_RCC_GPIOA_CLK_ENABLE();}
		if(PORT_B == GPIO_port){__HAL_RCC_GPIOB_CLK_ENABLE();}
		if(PORT_C == GPIO_port){__HAL_RCC_GPIOC_CLK_ENABLE();}
		if(PORT_D == GPIO_port){__HAL_RCC_GPIOD_CLK_ENABLE();}
		if(PORT_E == GPIO_port){__HAL_RCC_GPIOE_CLK_ENABLE();}
		if(PORT_F == GPIO_port){__HAL_RCC_GPIOF_CLK_ENABLE();}
		if(PORT_G == GPIO_port){__HAL_RCC_GPIOG_CLK_ENABLE();}

		PIN_DHT22_config.Pin = GPIO_num;
		PIN_DHT22_config.Mode = MODE_OUTPUT | OUTPUT_OD | EXTI_IT | TRIGGER_FALLING;
		PIN_DHT22_config.Pull = GPIO_PULLUP;
		PIN_DHT22_config.Speed = GPIO_SPEED_FREQ_HIGH;

		GPIO_TypeDef  * _HAL_PORT;
		_HAL_PORT=_HAL_PORT_DECODE(GPIO_port);

		//Cargar configuración PIN
		HAL_GPIO_Init(_HAL_PORT, &PIN_DHT22_config);

		//Activar interrupción dependiendo del pin elegido
		IRQn_Type IRQn;
		switch (GPIO_num){
			case GPIO_PIN_0:
				IRQn = EXTI0_IRQn;
			break;
			case GPIO_PIN_1:
				IRQn = EXTI1_IRQn;
			break;
			case GPIO_PIN_2:
				IRQn = EXTI2_IRQn;
			break;
			case GPIO_PIN_3:
				IRQn = EXTI3_IRQn;
			break;
			case GPIO_PIN_4:
				IRQn = EXTI4_IRQn;
			break;
			default:
				if(GPIO_num<GPIO_PIN_10){
					IRQn = EXTI9_5_IRQn;
				} else {
					IRQn = EXTI15_10_IRQn;
				}
			break;
		}

		HAL_NVIC_SetPriority(IRQn,0,0);
		NVIC_EnableIRQ(IRQn);

		Timer_Init();
	}
}



/**
 * @brief	Poner en bajo o liberar PIN
 * @param	Port. Valores válidos PORT_A a PORT_G
 * @param	Número de PIN
 * @param	Estado a setear. 0=>Bajo. 1=>Open.
 * @note	GPIO en modo open-drain
 */
static void GPIO_write(uint8_t _GPIO_port, uint16_t GPIO_num, bool_t GPIO_state){

	if(IS_GPIO_PIN(GPIO_num)){

		GPIO_TypeDef * GPIO_port;
		GPIO_port = _HAL_PORT_DECODE(_GPIO_port);

		if(GPIO_state) {
			HAL_GPIO_WritePin(GPIO_port, GPIO_num, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIO_port, GPIO_num, GPIO_PIN_RESET);
		}

	} else {
		/*Error en el número de PIN*/
	}
}




/*----------------------------------------- Interrupciones GPIO ------------------------------------------*/

/**
 * @brief	Manejador de interrupción EXIT0
 */
void EXTI0_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief	Manejador de interrupción EXIT1
 */
void EXTI1_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

/**
 * @brief	Manejador de interrupción EXIT2
 */
void EXTI2_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

/**
 * @brief	Manejador de interrupción EXIT3
 */
void EXTI3_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

/**
 * @brief	Manejador de interrupción EXIT4
 */
void EXTI4_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

/**
 * @brief	Manejador de interrupción EXIT9_5
 */
void EXTI9_5_IRQHandler(void){
	if(_DHT22.Pin<=GPIO_PIN_9 && _DHT22.Pin>=GPIO_PIN_5){
		HAL_GPIO_EXTI_IRQHandler(_DHT22.Pin);
	} else {
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	}
}

/**
 * @brief	Manejador de interrupción EXIT15_10
 */
void EXTI15_10_IRQHandler (void){
	if(_DHT22.Pin<=GPIO_PIN_15 && _DHT22.Pin>=GPIO_PIN_10){
		HAL_GPIO_EXTI_IRQHandler(_DHT22.Pin);
	} else {
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	}
}


/**
 * @brief	Control de interrupción por PIN
 * @param	Número de PIN
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==_DHT22.Pin){

		//Dependiendo el largo del bit, definir si se recibió 0 o 1
		uint32_t new_bit = __HAL_TIM_GET_COUNTER(&hTim2);
		_DHT22.status = DHT_READING;
		if(new_bit>T_corte){new_bit=1;}else{new_bit=0;}

		//Lectura de los bits de humedad y temperatura (primeros 4 bytes) y guardado en _data_crude
		if(T_Array_counter>=_inicio_bit_0 && T_Array_counter<32+_inicio_bit_0){
			new_bit = new_bit<<(31+_inicio_bit_0-T_Array_counter);
			_data_crude |= new_bit;
		}

		//Lectura de los bits de paridad (último byte)
		if(T_Array_counter>=32+_inicio_bit_0 && T_Array_counter<40+_inicio_bit_0){
			new_bit = new_bit<<(39+_inicio_bit_0-T_Array_counter);
			_data_validation |= new_bit;

			//Si se completó la lectura:
			if(T_Array_counter==39+_inicio_bit_0){
				cont_timer=0;
				_DHT22.status = DHT_READED;
			}
		}

		T_Array_counter++;
		if(T_Array_counter> 39+_inicio_bit_0){
			T_Array_counter=0;
		}

		//Recetear Timer para medir el nuevo tiempo entre flancos de un bit
		__HAL_TIM_SET_COUNTER(&hTim2,0);
	}
}



/*------------------------------------------------ TIMER -------------------------------------------------*/


/**
 * @brief	Inicializa el Timer 2
 * @note	Cuenta cada 1us e interrupción cada 1ms
 */
static void Timer_Init(void){
	__HAL_RCC_TIM2_CLK_ENABLE();
    hTim2.Instance = TIM2;
    hTim2.Init.Prescaler = 80-1;							/*Valor del contador del timer en us*/
    hTim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTim2.Init.Period = 1000-1;								/*Interrupción cada 1ms*/
    hTim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    hTim2.Init.RepetitionCounter = 0;
    hTim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&hTim2);

    //Selección del reloj interno
    TIM_ClockConfigTypeDef TimClock = {0};
    TimClock.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&hTim2, &TimClock);

	//Configurar reloj como master
	TIM_MasterConfigTypeDef TimMaster = {0};
	TimMaster.MasterSlaveMode = TIM_SLAVEMODE_DISABLE;
	TimMaster.MasterOutputTrigger = TIM_TRGO_RESET;
	HAL_TIMEx_MasterConfigSynchronization(&hTim2, &TimMaster);

	//Habilitar interrupción del timer
	HAL_NVIC_SetPriority(TIM2_IRQn,5,5);
	NVIC_EnableIRQ(TIM2_IRQn);

	HAL_TIM_Base_Start_IT(&hTim2);
}


/**
 * @brief	Manejador de interrupción por Timer2
 */
void TIM2_IRQHandler (void){
	HAL_TIM_IRQHandler(&hTim2);
}


/**
 * @brief	Control de interrupción por Timer2
 * @param	Manejador del Timer2
 * @Note	Ocurre una interrupcion cada 1ms
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	cont_timer++;

	//Desbordamiento en caso de que la lectura no se haya completado
	if(cont_timer>9000){
		cont_timer=0;
	}

	//Liberar pin para esperar respuesta del DHT22
	if(cont_timer>2501){
		T_Array_counter=0;
		GPIO_write(_DHT22.Port, _DHT22.Pin, 1);
	}else if(cont_timer>=2500){	//Iniciar lectura poniendo pin en bajo
		_data_crude=0;
		_data_validation=0;
		GPIO_write(_DHT22.Port, _DHT22.Pin, 0);
		T_Array_counter=0;
	}

	//Si no hubo flancos descendentes durante 1ms tras pedir el dato considerar DHT22 desconectado
	if(_DHT22.status == DHT_READING){_DHT22.status = ERR_DISCONECT;}
}
