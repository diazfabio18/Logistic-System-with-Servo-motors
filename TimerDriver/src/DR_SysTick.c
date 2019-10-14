#include "DR_SysTick.h"

uint8_t retorno = 0;
uint8_t cuenta = 0;
uint8_t pepe = 0xFF;
uint8_t flag_color=8;

void InicializarSysTick ( void )
{
	STRELOAD = TICK;	// a 100Mhz, el systick dara un 'tick' cada 1 ms
	STCURR = 0;	// Pongo el contador en cero
	STCTRL = 0x07;	//Inicio systick en bit0
					//Habilito interrupcion del systick en bit1
					//Habilito el uso el clk del cpu con el bit2
}

void SysTick_Handler(void) //funcion de interrupcion que se ejecuta cada 1ms
{
	static uint8_t i=0;
	//BarridoDisplay(); // hago el barrido de la expansion 2
	//Tecla = DriverTecladoHW(); //verifica y filtra el teclado 1x4 de la placa base
	DriverTecladoSW();

	if(cuenta == 1)
	{
		i++;
	}
	if(i>1500)
	{
		i=0;
		retorno = 1;
	}
	//barridoservo();
	//barridotimer();
	//if(play_sensor==1)
	//	barrido_sensor();
}
