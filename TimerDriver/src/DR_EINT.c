#include "DR_EINT.h"

void EXTINT_Inicializacion( uint8_t  puerto, uint8_t  pin, uint8_t  modo)
{
	SetPINSEL(puerto,pin,FUNCION_1); //setea uno de las 4 posibilidades como interrupcion externa
	pin -= 10;

	if(pin < 4)
	{
		ISER0 |= (0x01 << ( pin + 18));	//habilito la interrupcion en el NVIC
		EXTINT |= (0x01 << pin);	//limpio el flag ?

		if(modo)
		{
			EXTMODE |= (0x01 << pin);
			EXTPOLAR |= (0x01 << pin);
		}
		else
		{
			EXTMODE &= ~(0x01 << pin);
			EXTPOLAR &= ~(0x01 << pin);
		}
	}
}

void EINT3_IRQHandler(void) //funcion de interrupcion que se activa dependiendo del modo
{
	EXTINT |= (0x01 << 3); // limpio el flag

	/*Acciones a realizar*/
	//NOTA: para que el programa principal sepa alguna condicion impuesta por la interrupcion
	//se deberá usar variables globales
}
