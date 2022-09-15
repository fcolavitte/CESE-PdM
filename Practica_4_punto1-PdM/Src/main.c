 /*******************************************************************************
  *
  * 	CESE 18 - PdM
  * 	Facundo Colavitte
  *
  * 	Ejercicio 3 - Puntos 1 y 2
  *
  * 	Retardos no bloqueantes con secuencia de LEDs
  *
  *******************************************************************************/

#include "main.h"
#include "API_delay.h"

#define T_ANTIREBOTE 40

// UART handler declaration
UART_HandleTypeDef UartHandle;

//Functions prototypes
static void SystemClock_Config(void);
static void Error_Handler(void);


void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();		// debe leer las entradas, resolver la lógica de
								// transición de estados y actualizar las salidas
void buttonPressed();			// debe togglear el LED1
void buttonReleased();			// debe togglear el LED3


//Global Variables
typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

debounceState_t stateButton;
delay_t delay;

int main(void) {
  HAL_Init();
  SystemClock_Config(); // Configure the system clock to 180 MHz

  BSP_LED_Init(LED1);	//LED1=0
  BSP_LED_Init(LED2);	//LED2=1
  BSP_LED_Init(LED3);	//LED3=2
  for(int led=0;led<3;led++){BSP_LED_Off(led);}	//Inicializar LEDs en Off

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);	//Iniciar Pulsador
  debounceFSM_init();	//Iniciar estado del botón

  delayInit(&delay,T_ANTIREBOTE);

  while (1) {
	  debounceFSM_update();
  }
}


/*
 * @brief	Inicializa la máquina de estados del pulsador y la estructura
 * 			delay para el anti-rebote
 * @param	None
 * @return	None
 */
void debounceFSM_init(void){
	stateButton=BUTTON_UP;
}


/*
 * @brief	Actualiza estado del pulsador. Llama a las funciones buttonPressed()
 * 			y buttonReleased() según corresponde
 * @param	None
 * @return	None
 */
void debounceFSM_update(void){
	//T_ANTIREBOTE
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
					buttonPressed();
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
					buttonReleased();
				}
			}
		break;
		default:
			//>control de error<
			debounceFSM_init();
		break;
	}
}


/*
 * @brief	Alterna el estado del LED1
 * @param	None
 * @return	None
 */
void buttonPressed(void){
	BSP_LED_Toggle(LED1);
}


/*
 * @brief	Alterna el estado del LED3
 * @param	None
 * @return	None
 */
void buttonReleased(void){
	BSP_LED_Toggle(LED3);
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
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
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
