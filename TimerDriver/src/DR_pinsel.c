#include "DR_pinsel.h"

/*Funcion que configura un pin con alguna de sus 4 modos de operacion interna*/
void SetPINSEL (uint8_t puerto, uint8_t pin, uint8_t modo)
{
	//PINSEL[puerto*2 + (pin%16)] = (pin<16)? ((PINSEL[puerto*2] &= ~(0x03 << (pin*2))) | (modo << (pin*2))) : ((PINSEL[puerto*2 + 1]&= ~(0x03 << ((pin-16)*2))) |(modo << ((pin-16)*2)));
	if(pin<16)
	{
		PINSEL[puerto*2] &=~(0x03 << (pin*2)); //borro los 2 bit a escribir
		PINSEL[puerto*2] |= (modo << (pin*2));
	}
	else
	{
		pin-=16;
		PINSEL[(puerto*2) +1] &=~(0x03 << (pin*2));
		PINSEL[(puerto*2) +1] |= (modo << (pin*2));
	}
}
