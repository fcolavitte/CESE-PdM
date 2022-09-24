# PdM - CESE
#### Repositorio para practicas de la materia Programación de Microcontroladores de la Especialización Sistemas Embebidos
Workspace: PdM de CESE 18</br>
Alumno: Facundo Colavitte</br>
Profesor: Patricio Bos</br>
IDE: STM32CUBEIDE</br>
[![version](https://img.shields.io/badge/última_práctica-5.2-blue.svg)](https://github.com/fcolavitte/CESE-PdM/tree/main/Practica_5_punto2-PdM)
## Práctica 1
>Archivo: Practica_1-PdM
><details><summary>Descripción</summary>
>Programa que hace parpadear una secuencia de 3 leds con un tiempo en On y Off de 200ms en cada led.<br/>
>El pulsador cambia el sentido de la secuencia.
></details> 

## Práctica 2
>Archivo: Practica_2-PdM
><details><summary>Descripción</summary>
>Programa que utilice retardos no bloqueantes y haga titilar en forma periódica e independiente los tres leds de la placa NUCLEO-F429ZI de la siguiente manera:
><li>LED1: 100 ms</li>
><li>LED2: 500 ms</li>
><li>LED3: 1000 ms</li>
></details> 

## Práctica 3
>Archivo: Practica_3-PdM
><details><summary>Descripción</summary>
>Dentro de Drivers, se creó una carpeta API/src y API/inc y se colocó allí las funciones de retardos no bloqueantes.<br/>
>En main.c se realizó nuevamente la práctica 1 pero utilizando los retardos no bloqeantes incluidos en Drivers/API.
></details> 

## Práctica 4
>Archivos:
>- Practica_4_punto1-PdM 
>- Practica_4_punto2-PdM
><details><summary>Descripción</summary>
><h3>Punto 1</h3>Implementar una MEF anti-rebote que permita leer el estado del pulsador de la placa NUCLEO y alterne el estado de los LEDs 1 y 2 según se dé un flanco ascendente o descendente.
><h3>Punto 2</h3>Modularizar el punto 1 y agregar una función de parpadeo de delay no bloqueante para el LED2 de tiempo alternable entre 100 y 500ms según pulsador.
></details> 

## Práctica 5
>Archivos:
>- Practica_5_punto1-PdM 
>- Practica_5_punto2-PdM>
><details><summary>Descripción</summary>
><h3>Punto 1</h3>Generar una librería para comunicación UART.
><h3>Punto 2</h3>Utilizar la librería para UART y la librería de la práctica 4 de detección del botón para avisar vía UART los flancos ascendentes y descendentes del pulsador incluido dentro de la placa núcleo.
></details> 

