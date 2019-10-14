/*
===============================================================================
 Name        : TimerDriver.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include "DR_PLL.h"
#include "DR_gpio.h"
#include "DR_Timer.h"
#include "PR_Teclado.h"
#include "DR_Systick.h"
#include "PR_UART.h"

void TimerInterrupt(uint8_t match);
void TimerInterrupt2(uint8_t match);
//void variacion_pulso(uint8_t match);
void mide_periodo(uint8_t match);

#define SERVO1	0
#define SENSOR	1
#define SERVO2	2

uint8_t valor = 0;

/******/
void InicializarSENSOR(void);
uint8_t barrido_sensor(void);

#define	S0	PORT0,20
#define	S1	PORT3,25
#define	S2	PORT1,27
#define	S3	PORT1,24
#define	OUT_SENSOR	PORT1,21
#define ENABLE_SENSOR	PORT1,18

#define	LOW	0
#define	HIGH	1
#define	COLOR_ROJO	1
#define	COLOR_VERDE	2
#define	COLOR_AZUL	3

#define TIMER_1 1
#define CANAL1 1
#define	AMBOS_FLANCOS 0x03
/*****/
#define	COMUNICACION	0
#define	ESPERANDO	1
#define	BOTON	2
#define	SENSOR_COLOR	3
#define	ALMACEN	4

int main(void)
{
	InicializarPLL();
	InicializarSENSOR();
	UART0_Inicializacion();
	InicializarSysTick();
	InicializarTeclados();

	SetPINSEL(0,19,0); //5
	SetDIR(0, 19, SALIDA);
	SetPIN(0, 19, 0);

	SetPINSEL(1,20,0); //4
	SetDIR(1, 20, SALIDA);
	SetPIN(1, 20, 0);

	SetPINSEL(1,18,0);//15
	SetDIR(1, 18, SALIDA);
	SetPIN(1, 18, 0);

	SetPINSEL(1,23,0);//3
		SetDIR(1, 23, SALIDA);
		SetPIN(1, 23, 0);

	/***configuro servo de arriba con TIMER 0**/

	TIMER_Init(0);
	TIMER_SetRunningFrequency(0, 40000);
	TIMER_SetMatchCallback(0, TimerInterrupt);
	TIMER_SetMatch(0, 0, 200, TIMER_MATCH_INTERRUPT | TIMER_MATCH_RESET);
	TIMER_SetMatch(0, 1, 25, TIMER_MATCH_INTERRUPT);
	TIMER_Start(0);

	/***configuro servo de abajo con TIMER 1 **/
	TIMER_Start(1);

	uint8_t dato;
		    // Enter an infinite loop, just incrementing a counter
	/**************************/

/*
		    while(1)
		    {
		    	if(UART0_PopRX(&dato))
		        {
		    		if((dato == 'k') || (dato == 'K'))
		    		{
		    			UART0_PushTX(dato);
		    			TIMER_SetMatch(0, 1, 9, TIMER_MATCH_INTERRUPT);
		    		}
		    		else if((dato == 'e') || (dato == 'E'))
		    		{
		    			UART0_PushTX('B');
		    			TIMER_SetMatch(0, 1, 25, TIMER_MATCH_INTERRUPT);
		    		}
		        	dato = 0;
		        	//UART0_PushTX('K');
		        }

		    	uint8_t tec = GetKey();

		    	if(tec == SW1)
		    	{
		    		TIMER_SetMatch(0, 1, 9, TIMER_MATCH_INTERRUPT);
		    		//TIMER_SetMatch(1, 1, 9, TIMER_MATCH_INTERRUPT);
		    	}
		    	else if(tec == SW4)
		    	{
		    		TIMER_SetMatch(0, 1, 14, TIMER_MATCH_INTERRUPT);
		    		//TIMER_SetMatch(1, 1, 14, TIMER_MATCH_INTERRUPT);
		    	}
		    	else if(tec == SW7)
		    	{
		    		//TIMER_SetMatch(0, 1, 19, TIMER_MATCH_INTERRUPT);
		    		TIMER_SetMatch(1, 1, 19, TIMER_MATCH_INTERRUPT);
		    	}
		    	else if(tec == SW10)
		    	{
		    		//TIMER_SetMatch(0, 1, 25, TIMER_MATCH_INTERRUPT);
		    		TIMER_SetMatch(1, 1, 25, TIMER_MATCH_INTERRUPT);
		    	}

		    }

*/
		    	/**************************/

	/***LA MAQUINA DEFINITIVA***/

	static uint8_t estado = COMUNICACION;
	TIMER_SetMatch(0, 1, 9, TIMER_MATCH_INTERRUPT); //servo arriba
	TIMER_SetMatch(1, 1, 9, TIMER_MATCH_INTERRUPT); //servo abajo
	//valor= 0 ; //servo arriba
	while(1)
	{
		//UART0_PushTX('R');
		switch(estado)
		{
			case COMUNICACION:
				//UART0_PushTX('T');
				//UART0_PushTX('j');
				UART0_PopRX(&dato);
				if((dato == 'k')|| (dato == 'K'))
				{
					UART0_PushTX(dato);
					estado = ESPERANDO;

					//UART0_PushTX('e');
					//UART0_PushTX('s');
//					//UART0_PushTX('p');
				}
				break;
			case ESPERANDO:

				TIMER_SetMatch(0, 1, 25, TIMER_MATCH_INTERRUPT); //servo arriba
				TIMER_SetMatch(1, 1, 25, TIMER_MATCH_INTERRUPT); //servo abajo

				if((dato == 'e') || (dato == 'E'))
				{
					UART0_PopRX(&dato);
					if((dato == 's') || (dato == 'S'))
					{
							//UART0_PushTX(dato);
							estado = COMUNICACION;

							//UART0_PushTX('c');
							//UART0_PushTX('o');
							//UART0_PushTX('m');
					}
					else if((dato == 'd') || (dato == 'D'))
					{
							//UART0_PushTX(dato);
							//dato = dato1; //e pasa a D
							estado = ESPERANDO; //se mantiene en este estado
					}
					else
					{
						estado = BOTON;
						//valor = 0; //servo arriba
						//UART0_PushTX('b');
						//UART0_PushTX('o');
						//UART0_PushTX('t');

					}

				}
				else //si es D o K
				{

					UART0_PopRX(&dato);
					if((dato == 'e') || (dato == 'E'))
					{
						//UART0_PushTX(dato);
						estado = BOTON;

						//UART0_PushTX('b');
						//UART0_PushTX('o');
						//UART0_PushTX('r');
					}
					else if((dato == 's') || (dato == 'S'))
					{
						//UART0_PushTX(dato);
						estado = COMUNICACION;
						//UART0_PushTX('c');
						//UART0_PushTX('o');
						//UART0_PushTX('n');
					}
				}
				break;
			case BOTON:
				if(GetKey() == SW10) //comienza el servo a funcionar a 90
				{
					estado = SENSOR_COLOR;
//					UART0_PushTX('s');
					//UART0_PushTX('e');
					//UART0_PushTX('n');

					TIMER_SetMatch(0, 1, 19, TIMER_MATCH_INTERRUPT); //servo arriba
					//prender sensor (o prender leds sensor)
				}
				break;
			case SENSOR_COLOR:
				pepe = barrido_sensor();
				//barrido sensor; //debo guardar en esta funcion el estado del ultimo sw1,4,o 7
				if(GetKey() == SW10)
				{
					estado = ALMACEN;
					//UART0_PushTX('a');
					//UART0_PushTX('l');
					//UART0_PushTX('m');

					//servo arribaa 180
					TIMER_SetMatch(0, 1, 9, TIMER_MATCH_INTERRUPT);

					switch(flag_color)
					{
						case SW1:
							UART0_PushTX('R');
						TIMER_SetMatch(1, 1, 9, TIMER_MATCH_INTERRUPT);
						break;
						case SW4:
						TIMER_SetMatch(1, 1, 19, TIMER_MATCH_INTERRUPT);
						UART0_PushTX('G');
						break;
						case SW7:
						TIMER_SetMatch(1, 1, 25, TIMER_MATCH_INTERRUPT);
						UART0_PushTX('B');
						break;
					}

					//TIMER_SetMatch(0, 1, ?, TIMER_MATCH_INTERRUPT);
					 cuenta = 1;
				}
				break;
			case ALMACEN:
				//if(GetKey() == SW10)
				if(GetKey() == SW10)
				{
					estado = ESPERANDO;
					//UART0_PushTX('e');
						//			UART0_PushTX('s');
						//			UART0_PushTX('p');
						//							UART0_PushTX('e');
						//							UART0_PushTX('r');
				}
				/*if(retorno == 1)
				{
					cuenta = 0;
					retorno = 0;
				estado = ESPERANDO;
				UART0_PushTX('e');
				UART0_PushTX('s');
				UART0_PushTX('k');
				//dato = 'e';
								}*/
				break;
		}
	}

}

void TimerInterrupt(uint8_t match)
{
	if(match & 0x01)		//match 0
			SetPIN(0, 19, 1);

	else if(match & 0x02)	//match 1
		SetPIN(0, 19, 0);
}

void TimerInterrupt2(uint8_t match)
{
	if(match & 0x01)		//match 0
			SetPIN(1,23,1);
	else if(match & 0x02)	//match 1
			SetPIN(1,23,0);
}

void InicializarSENSOR(void)
{
	SetPINSEL(0,19,FUNCION_GPIO);
	SetDIR(0,19,SALIDA);
	SetPIN(0,19,LOW);

	SetPINSEL(S0,FUNCION_GPIO);
	SetPINSEL(S1,FUNCION_GPIO);
	SetPINSEL(S2,FUNCION_GPIO);
	SetPINSEL(S3,FUNCION_GPIO);

	SetDIR(S0,SALIDA);
	SetDIR(S1,SALIDA);
	SetDIR(S2,SALIDA);
	SetDIR(S3,SALIDA);

	SetPIN(S0,LOW);
	SetPIN(S1,LOW);
	SetPIN(S2,LOW);
	SetPIN(S3,LOW);

	SetPINSEL(ENABLE_SENSOR,FUNCION_GPIO);
	SetDIR(ENABLE_SENSOR,SALIDA);
	//SetPIN(ENABLE_SENSOR,LOW); //sensor encendido
	SetPIN(ENABLE_SENSOR,HIGH);

	SetPINSEL(OUT_SENSOR,0);
	SetDIR(OUT_SENSOR,ENTRADA);
	SetMODE(OUT_SENSOR, PULLDOWN);

	TIMER_Init(TIMER_1);

	TIMER_SetRunningFrequency(TIMER_1,40000); ///VER 2DO ARGUMENTO, DEBO CONFIGURR A 100US

	TIMER_SetMatchCallback(1, mide_periodo);
	TIMER_SetMatch(1, 0, 200, TIMER_MATCH_INTERRUPT | TIMER_MATCH_RESET);
	TIMER_SetMatch(1, 1, 9, TIMER_MATCH_INTERRUPT );
}

uint8_t barrido_sensor(void)
{
	uint32_t pulso=0;
	uint8_t tec=1;

	static uint8_t color = COLOR_ROJO;

	tec = DriverTecladoHW();
	if(tec == SW1)
	{
		color = COLOR_ROJO;
		flag_color = SW1;
	}
	if(tec == SW4)
	{
		color = COLOR_VERDE;
		flag_color = SW4;
	}
	if(tec == SW7)
	{
		color = COLOR_AZUL;
		flag_color = SW7;
	}

	/** MODO frecuencia 2% **/
	static uint8_t flag = 0;
	if(flag==0)
	{
	SetPIN(S0,LOW);
	SetPIN(S1,HIGH);
	flag=1;
	}

	switch(color)
	{
		case COLOR_ROJO:
			/** MODO filtro ROJO**/
			SetPIN(S2,LOW);
			SetPIN(S3,LOW);
			break;
		case COLOR_VERDE:
			/** MODO filtro VERDE**/
			SetPIN(S2,HIGH);
			SetPIN(S3,HIGH);
			break;
		case COLOR_AZUL:
			/** MODO filtro AZUL**/
			SetPIN(S2,LOW);
			SetPIN(S3,HIGH);
			break;
	}

	return tec;
}


void mide_periodo(uint8_t match) //funcion interrupcion del sensor
{


		//if(match & 0x01)		//match 0
		//{
		//		SetPIN(1, 20, 1);
		//}
		//else if(match & 0x02)	//match 1
		//{
		//	SetPIN(1, 20, 0);
		//}

	/*if(GetPIN(OUT_SENSOR,1) == 1)
	{
		UART0_PushTX('L');
	}*/

	if(match & 0x01)		//match 0
				SetPIN(1,23,1);
		else if(match & 0x02)	//match 1
				SetPIN(1,23,0);

}

