 /*******************************************************************************
  *
  * 	CESE 18 - PdM
  * 	Facundo Colavitte
  *
  * 	Ejercicio 1 - Puntos 1 y 2
  *
  * 	Hacer parpadear los leds de la placa en secuencia
  * 	Cambiar la secuencia con el pulsador azul
  *
  *******************************************************************************/

#include "main.h"

//Macro para saber la longitud de un array
#define ARRAYLENGTH(x) (sizeof x/sizeof x[0])

#define tiempo_antirebote 30
#define DELAY 200	//Tiempo en encendido y apagado de cada led

// UART handler declaration
UART_HandleTypeDef UartHandle;

//Functions prototypes
static void SystemClock_Config(void);
static void Error_Handler(void);
void parpadear(int);
void esperar(unsigned int);

//Global Variables
int t_button = 0;	//Tiempo consecutivo del pulsador presionado
int secuencia = 0;	//Secuencia elegida
int estado_button=0;//estado: 0>suelto, 1>presionado


int main(void) {
  HAL_Init();
  SystemClock_Config(); // Configure the system clock to 180 MHz

  BSP_LED_Init(LED1);	//LED1=0
  BSP_LED_Init(LED2);	//LED2=1
  BSP_LED_Init(LED3);	//LED3=2
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  //Inicializar LEDs en Off
  for(int led=0;led<3;led++){BSP_LED_Off(led);}

  //Secuencias
  int secuencia_1[]={1,2,3};
  int secuencia_2[]={1,3,2};
  //int secuencia_2[]={1,1,3,3,2,2};	//Ejemplo de secuencias más largas (el codigo lo permite)
  //int secuencia_2[]={1,2,3,2};		//Ejemplo de secuencias más largas (el codigo lo permite)

  int i=0;//Variable auxiliar para recorrer la secuencia
  int led;//N° de LED a encender

  while (1)
  {
	  if(!secuencia){	//Ejecutar secuencia_1
		  if(i>ARRAYLENGTH(secuencia_1)-1){i=0;}
		  led = secuencia_1[i];
	  } else {	//Ejecutar secuencia_2
		  if(i>ARRAYLENGTH(secuencia_2)-1){i=0;}
		  led = secuencia_2[i];
	  }
	  parpadear(led);
	  i++;	//Siguiente led en la secuencia
  }
}


void parpadear(int led) {
	BSP_LED_On(led-1);	//"led-1" para pasar de {LED1,LED2,LED3} a {0,1,2} (números asignados a cada LEDx)
	esperar(DELAY);
	BSP_LED_Off(led-1);
	esperar(DELAY);
}


void esperar(unsigned int delay) {
	for(unsigned int t=0;t<delay;t++) {
		HAL_Delay(1);

		//Comprobar estado del botón
		if(BSP_PB_GetState(BUTTON_USER)){	//Botón presionado
			if(estado_button==0){			//Si el botón no se considera todavía presionado (por rebotes)
				t_button++;
			}
			if (t_button>tiempo_antirebote){	//milisegundos que tiene que estar presionado el botón (antirebote)
				t_button=0;
				estado_button=1;								//Considerar botón presionado (sin rebotes)
				if(secuencia){secuencia=0;}else{secuencia=1;}	//Cambiar secuencia
			}
		} else {	//Se soltó botón
			t_button=0;			//Resetear tiempo del boton presionado
			estado_button=0;	//Regresar estado macro del botón a suelto
		}
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
