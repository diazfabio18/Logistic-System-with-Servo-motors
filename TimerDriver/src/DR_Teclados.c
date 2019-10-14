#include "DR_Teclados.h"

uint8_t volatile Tecla = NO_KEY;

void InicializarTeclados( void )
{
	//Inicializo las 4 teclas del teclado del Infotronic como GPIO
	SetPINSEL(KEY0,FUNCION_GPIO); //SW1
	SetPINSEL(KEY1,FUNCION_GPIO);
	SetPINSEL(KEY2,FUNCION_GPIO);
	SetPINSEL(KEY3,FUNCION_GPIO);

	//Configuro las 4 teclas como ENTRADA
	SetDIR(KEY0,ENTRADA);
	SetDIR(KEY1,ENTRADA);
	SetDIR(KEY2,ENTRADA);
	SetDIR(KEY3,ENTRADA);

	//Las configuro como pullup
	SetMODE(KEY0,PULLUP);
	SetMODE(KEY1,PULLUP);
	SetMODE(KEY2,PULLUP);
	SetMODE(KEY3,PULLUP);
}

uint8_t DriverTecladoHW(void)
{
	/*Analizo que tecla se pulsó*/
	if( GetPIN(KEY0,BAJO) )
		return SW1;
	if( GetPIN(KEY1,BAJO))
		return SW4;
	if( GetPIN(KEY2,BAJO))
		return SW7;
	if( GetPIN(KEY3,BAJO))
		return SW10;

	return NO_KEY; // de lo contrario, retorno que no hubo tecla
}

void DriverTecladoSW ( void )	//Funcion que hace de filtro antirebotes
{
	static uint8_t contador = 0;
	static uint8_t key_anterior = NO_KEY;
	uint8_t key;
	key = DriverTecladoHW(); // almaceno la tecla almacenada

	if(key == key_anterior) //si hubo tecla y el contador no llego al maximo
	{
		if(contador < CANT_REBOTES)
		{
			contador++;
			if(contador == CANT_REBOTES) //si pasó por cant_rebotes veces y detecto la misma tecla, se valida
			{
				Tecla = key;	//igualo la tecla analizada a la variable global
			}
		}
	}
	//else if(key == NO_KEY)
	if(key==NO_KEY)
	{
		contador = 0; //RESETEO CONTADOR
	}

	key_anterior = key;
}
