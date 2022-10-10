/*******************************************************************************
*
* 	CESE 18 - PdM
* 	Facundo Colavitte
*
* 	Trabajo Final PdM - CESE18
*
* 	Sistema de control de temperatura con DHT22
*
*******************************************************************************/

#include "main.h"
#include "API_uart.h"
#include "API_debounce.h"
#include "API_dht22.h"
#include "API_num_strings.h"
#include "API_pwm.h"
#include <stdbool.h>

#define sist_agua  LED2 //LED Azul

//Functions prototypes
static void SystemClock_Config(void);
static void Error_Handler(void);

void MEF_UART_update(void);
void MEF_sistem_update(void);


//Global Variables
static enum{
	MEF_CONFIG		= 0,
	MEF_OFF			= 1,
	MEF_PWM			= 2,
	MEF_ON			= 3,
	MEF_EMERGENCIA	= 4,
	MEF_ENCLAVADO	= 5,
	MEF_DHT_ERR		= 6
}MEF_state;	//MEF sistema principal

static enum{
	SEND_TEMP,
	RECEIVE_T_MIN,
	RECEIVE_T_MAX,
	RECEIVE_T_CRIT,
	SEND_T_MIN,
	SEND_T_MAX,
	SEND_T_CRIT,
    SEND_CONFIG
}uart_input_state;	//MEF comunicación UART

uint8_t temp_min=23;
uint8_t temp_max=25;
uint8_t temp_critica=27;
uint8_t PWM_ventilador=0;
float temp = 0;

/**Tiempo en ms entre cada catualización de dato de temperatura enviado por UART*/
delay_t delay_print_UART;

/**Tiempo a permanecer en el estado PWM antes de pasar a ON u OFF*/
delay_t delay_in_PWM;

/**Tiempo en ms en que el sistema estará en modo emergencia, con el sistema de agua activo*/
delay_t delay_emergencia;

/**Variable para almacenar caracteres recibidos por UART*/
uint8_t character[2];



int main(void) {
  HAL_Init();
  SystemClock_Config();

  //Inicializar LEDs y pulsador de la placa NUCLEO
  BSP_LED_Init(sist_agua);	/*LED Azul, simula ser el sistema de agua*/
  BSP_LED_Init(LED3);		/*LED ERROR*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  //Iniciar MEF del pulsador
  debounceFSM_init();

  //Iniciar UART
  if(uartInit()!=1){
	  Error_Handler();
  }
  delayInit(&delay_print_UART, 1000);

  //Iniciar DHT22
  if(DHT22_init(GPIO_PIN_4, PORT_E)!=1){
	  Error_Handler();
  }

  //Configurar pin LED1(ventilador) como PWM
  if(pwm_init()!=1){
	  Error_Handler();
  }

  //Estado inicial de la MEF
  MEF_state = MEF_CONFIG;

  character[0] = 0;
  character[1] = 0;

  while (1) {

	  //Actualizar lectura del pulsador
	  debounceFSM_update();

	  //Actualizar estado de la mEF que controla comunicación UART
	  MEF_UART_update();

	  //Actualizar estado del sistema de control de temperatura
	  MEF_sistem_update();
  }
}






/**
 * @brief actualizar MEF de HMI UART
 */
void MEF_UART_update(void){

	//Recibir dos caracteres por UART
	uartReceiveStringSize(character,2);

	switch (uart_input_state){
	case SEND_TEMP:
		//Mostrar temperatura y estado de la MEF del sistema
		if(delayRead(&delay_print_UART)){
			if(DHT22_get_temp_string()[0]=='E'){
				MEF_state=100;
			}
			uartSendString((uint8_t *)"Temperatura: ");
			uartSendString(DHT22_get_temp_string());
			uartSendString((uint8_t *)"- MEF: ");
			uint8_t MEF_string = MEF_state+'0';
			uartSendStringSize(&MEF_string,1);
			uartSendString((uint8_t *)"\r\n");
		}

		//Si se recibió algun comando(Tm, TM , Tc o C) cambiar estado de la MEF de UART
		if(character[0]=='T'){
			if(character[1]=='m'){
				uart_input_state = SEND_T_MIN;
			}
			if(character[1]=='M'){
				uart_input_state = SEND_T_MAX;

			}
			if(character[1]=='c'){
				uart_input_state = SEND_T_CRIT;

			}
		}
		if(character[0]=='C'){
			uart_input_state = SEND_CONFIG;
		}
		character[0]=0;
		character[1]=0;
		break;

	case SEND_T_MIN:
		uartSendString((uint8_t *)"\r\nTemp_min = ");
		uartSendString(uint_to_string(temp_min));
		uartSendString((uint8_t *)". nuevo valor: ");
		uart_input_state = RECEIVE_T_MIN;
		break;

	case RECEIVE_T_MIN:
		if(character[0]>='0'&&character[0]<='9'&&character[1]>='0'&&character[1]<='9'){
			temp_min = (character[0]-'0')*10 + character[1]-'0';
			uartSendString(uint_to_string(temp_min));
			uartSendString((uint8_t *)"\r\n");
			uart_input_state = SEND_TEMP;
		}
		break;

	case SEND_T_MAX:
		uartSendString((uint8_t *)"\r\nTemp_max = ");
		uartSendString(uint_to_string(temp_max));
		uartSendString((uint8_t *)". nuevo valor: ");
		uart_input_state = RECEIVE_T_MAX;
		break;

	case RECEIVE_T_MAX:
		if(character[0]>='0'&&character[0]<='9'&&character[1]>='0'&&character[1]<='9'){
			temp_max = (character[0]-'0')*10 + character[1]-'0';
			uartSendString(uint_to_string(temp_max));
			uartSendString((uint8_t *)"\r\n");
			uart_input_state = SEND_TEMP;
		}
		break;

	case SEND_T_CRIT:
		uartSendString((uint8_t *)"\r\nTemp_crit = ");
		uartSendString(uint_to_string(temp_critica));
		uartSendString((uint8_t *)". nuevo valor: ");
		uart_input_state = RECEIVE_T_CRIT;
		break;

	case RECEIVE_T_CRIT:
		if(character[0]>='0'&&character[0]<='9'&&character[1]>='0'&&character[1]<='9'){
			temp_critica = (character[0]-'0')*10 + character[1]-'0';
			uartSendString(uint_to_string(temp_critica));
			uartSendString((uint8_t *)"\r\n");
			uart_input_state = SEND_TEMP;
		}
		break;

	case SEND_CONFIG:
		uartSendString((uint8_t *)"\r\nTemp_min = ");
		uartSendString(uint_to_string(temp_min));
		uartSendString((uint8_t *)"\r\nTemp_max = ");
		uartSendString(uint_to_string(temp_max));
		uartSendString((uint8_t *)"\r\nTemp_crit = ");
		uartSendString(uint_to_string(temp_critica));
		uartSendString((uint8_t *)"\r\n");
		uart_input_state = SEND_TEMP;
		break;
	}

}




/**
 * @brief actualizar MEF del sistema de control de temperatura.
 * @note control led verde(ventilador) y azul(sistema de agua)
 */
void MEF_sistem_update(void){

	temp = DHT22_get_temp();

	switch(MEF_state){
	case MEF_CONFIG:
		pwm_on(0);
		BSP_LED_Off(sist_agua);
		if(delayRead(&delay_print_UART)){
			uartSendString((uint8_t *)"\r\n>> Conectando con DHT22...\r\n");
		}
		//Si hubo error, la temperatura en formato comienza con 'E'
		if(DHT22_get_temp_string()[0]!='E'){
			uartSendString((uint8_t *)">> DHT22 conectado.\r\n");
			uartSendString((uint8_t *)">> Iniciando MEF.\r\n");
			MEF_state = MEF_OFF;
		}
		break;

	case MEF_OFF:
		pwm_on(0);
		BSP_LED_Off(sist_agua);
		if(temp>temp_min){
			delayInit(&delay_in_PWM, 3000);
			MEF_state = MEF_PWM;
		}
		break;

	case MEF_PWM:
		pwm_on(50);
		BSP_LED_Off(sist_agua);
		if(delayRead(&delay_in_PWM)){
			if(temp<temp_min){MEF_state = MEF_OFF;}
			if(temp>temp_max){MEF_state = MEF_ON;}
		}
		break;

	case MEF_ON:
		pwm_on(100);
		BSP_LED_Off(sist_agua);
		if(temp<temp_max){
			delayInit(&delay_in_PWM, 3000);
			MEF_state = MEF_PWM;
		}
		if(temp>temp_critica){
			delayInit(&delay_emergencia, 5000);
			MEF_state = MEF_EMERGENCIA;
		}
		break;

	case MEF_EMERGENCIA:
		pwm_on(0);
		BSP_LED_On(sist_agua);
		if(delayRead(&delay_emergencia)){
			MEF_state = MEF_ENCLAVADO;
		}
		break;

	case MEF_ENCLAVADO:
		pwm_on(0);
		BSP_LED_Off(sist_agua);
		if(readKey()){
			MEF_state = MEF_CONFIG;
		}
		break;

	default:
		uartSendString((uint8_t *)"\r\n! Error en la MEF. Reiniciando...\r\n");
		MEF_state = MEF_CONFIG;
		break;
	}

}









/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while (1) {
	  BSP_LED_Toggle(LED3);
	  HAL_Delay(1000);
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
