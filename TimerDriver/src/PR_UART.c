//#include "../../DRIVERS/inc/DR_UART0.h"
#include "PR_UART.h"

void comunicacion_serie()
{

}

uint8_t UART0_PopRX( uint8_t *dato)	// funcion que usa la aplicacion para leer el buffer rx
{
	if(cant_datosRx)
	{
		*dato = bufferRx[out_rx];
		out_rx++;

		if(out_rx == TAM_BUFFERRX)
		{
			out_rx = 0;
		}

		cant_datosRx--;

		return OK;
	}
	return VACIO;
}


uint8_t UART0_PushTX( uint8_t dato ) //funcion que usa la aplicacion para escribir el buffer tx
{
	if(cant_datosTx < TAM_BUFFERTX) // si el buffer no esta lleno
	{
		bufferTx[in_tx] = dato;
		in_tx++;

		if(in_tx == TAM_BUFFERTX)
			in_tx = 0;

		cant_datosTx++;

		if(!transmitiendo) //si aun no se trasmitio nada
		{
			UART0_PopTX(&dato);	//el driver toma el dato
			enviar(dato);
			transmitiendo = 1;
		}
		return OK;
	}
	return LLENO;
}
