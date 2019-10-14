#include "DR_UART0.h"

uint8_t in_tx = 0;
uint8_t out_tx = 0;

uint8_t cant_datosRx = 0;
uint8_t cant_datosTx = 0;

uint8_t transmitiendo = 0;

uint8_t out_rx = 0;
uint8_t in_rx = 0;

uint8_t bufferRx[TAM_BUFFERRX];
uint8_t bufferTx[TAM_BUFFERTX];

void UART0_Inicializacion ()
{
	PCONP |= (0x01<<3);	// Enciendo la UART
	PCLKSEL0 &= ~(0x03 << 6); // divisor de clock a 25MHz

	//PINSEL[0] &= ~(0x00000003 << 4);
	//PINSEL[0] |= (0x00000001 << 4);

	//PINSEL[0] &= ~(0x00000003 << 6); //tx0
	//PINSEL[0] |= (0x00000001 << 6); //rx0

	SetPINSEL(PORT0,2,FUNCION_1); // TX UART0
	SetPINSEL(PORT0,3,FUNCION_1);	// RX UART0

	U0LCR = 0x03;	// Configuracion 8,N,1
	U0LCR |= (0x80);	// DLAB=1;
	//U0DLL = 7;	//configuro velocidad a baudios
	U0DLL = 0xA3; //velocidad a 9600 baudios
	//U0DLL = 0x0E; //velocidad a 115200 baudios
	U0DLM = 0;
	U0LCR &= ~(0x80);	//DLAB=0
	U0IER = 0x03;	// Activo interrupciones del periferico UART
	ISER0 |= (0x01 << 5);	// Habilito registros en NVIC
}

uint8_t UART0_PushRX( uint8_t dato ) // funcion que usa el driver para enviarle informacion a la aplicacion
{
	if(cant_datosRx < TAM_BUFFERRX) //si el buffer de datos no esta lleno
	{
		bufferRx[in_rx] = dato; //almacena el dato en el buffer
		in_rx++;				//e incrementa el indice actual del vector bufferRx

		if(in_rx == TAM_BUFFERRX) //si el indice llegó al final y por tanto se desborda al incrementarse
		{
			in_rx = 0; // resetea el indice, haciendo un buffer circular
		}
		cant_datosRx++; //incrementa la cantidad total de datos en el buffer Rx
		return OK; // retorna con exito si se incrementó
	}
	return LLENO; // de lo contrario retorna que el buffer está completo
}

uint8_t UART0_PopTX(uint8_t *dato )	//funcion que usa el driver para leer lo que transmite la aplicacion
{
	if(cant_datosTx)  //si hay datos en el buffer
	{
		*dato = bufferTx[out_tx]; //el dato almacenado en el buffer se almacena en una variable
		out_tx++; //e incremento el indice del vector bufferTx

		if(out_tx == TAM_BUFFERTX) //si el indice supero el tamaño maximo de la cola circular
			out_tx = 0; //retorna a cero

		cant_datosTx--; //decremento la cantidad de datos totales
		return OK;
	}
	return VACIO;
}


void enviar(uint8_t dato)
{
	U0THR = dato;
}

void UART0_IRQHandler(void) // funcion de la UART0 una vez que hubo interrupcion
{
	uint8_t iir, dato, flag;

	do{
		iir = U0IIR;
			/* Led el registro IIR y guardo su valor en otra variable ya que este registro se borra una vez leido*/

		//flag = UART0_PopTX(&dato);

		if(iir & 0x02) //bit THRE: el uC UART termino de enviar un dato
		{
			flag = UART0_PopTX(&dato);
			if(flag==OK) //si el driver pudo tomar un dato (es decir, si hay dato a trasmitir)
				U0THR = dato; //dato es guardado en el registro U0THR
			else
				transmitiendo = 0; //deja de transmitir
		}

		if(iir & 0x04) // bit RBR: el uC UART termino de recibir un dato
		{
			UART0_PushRX((uint8_t)U0RBR); //envio al bufferRx el dato recibido para que la aplicacion lo lea
		}

	}while(! (iir & 0x01) ); //si hay otra interrupcion pendiente, sigue en el bucle
}
