/*******************************************************************************************************************************//**
 *
 * @file		DR_UART0.h
 * @brief		Breve descripción del objetivo del Módulo
 * @date		Fecha de creacion del archivo 10/06/2018
 * @author		Fabio Leonel Diaz
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MODULO
 **********************************************************************************************************************************/

#ifndef DRIVERLPC_DR_UART0_H_
#define DRIVERLPC_DR_UART0_H_

/***********************************************************************************************************************************
 *** INCLUDES GLOBALES
 **********************************************************************************************************************************/

#include "DR_PLL.h"
#include "DR_pinsel.h"
#include "DR_EINT.h"

/***********************************************************************************************************************************
 *** DEFINES GLOBALES
 **********************************************************************************************************************************/
#define TAM_BUFFERRX	200
#define TAM_BUFFERTX	200

#define OK	1
#define LLENO	0
#define VACIO	0

//!< ///////////////////   UART0   //////////////////////////
//!< 0x4000C000UL : Registro de control de la UART0:
#define		UART0	( (__RW uint32_t *) 0x4000C000UL )

//!< Registros de la UART0:
#define		U0THR		UART0[0]
#define		U0RBR		UART0[0]
#define		U0DLL		UART0[0]

#define		U0DLM		UART0[1]
#define		U0IER		UART0[1]

#define		U0IIR		((__R uint32_t *)UART0)[2]
//#define		U0IIR		((__R uint32_t *) 0x4000C008UL)

#define		U0FCR		((__W uint32_t *)UART0)[2]

#define		U0LCR		UART0[3]
//!< posición 4 no definida [consultar pag. 300 user manual LPC1769]
#define		U0LSR		UART0[5]
//!< posición 6 no definida [consultar pag. 300 user manual LPC1769]
#define		U0SCR		UART0[7]

//!< ///////////////////   UART1   //////////////////////////
//!< 0x40010000UL : Registro de control de la UART1:
#define		UART1	( (__RW uint32_t *) 0x40010000UL )

//!< Registros de la UART1:
#define		U1THR		UART1[0]
#define		U1RBR		UART1[0]
#define		U1DLL		UART1[0]

#define		U1DLM		UART1[1]
#define		U1IER		UART1[1]

#define		U1IIR		UART1[2]
#define		U1FCR		UART1[2]

#define		U1LCR		UART1[3]
#define		U1MCR		UART1[4]
#define		U1LSR		UART1[5]
#define		U1MSR		UART1[6]
#define		U1SCR		UART1[7]

//!< //////////BITS DE ANALISIS para todas las UARTs   ////////////////////
#define 	IER_RBR		0x01
#define 	IER_THRE	0x02
#define 	IER_RLS		0x04

#define 	IIR_PEND	0x01
#define 	IIR_RLS		0x06
#define 	IIR_RDA		0x04
#define 	IIR_CTI		0x0C
#define 	IIR_THRE	0x02

#define 	LSR_RDR		0x01
#define 	LSR_OE		0x02
#define 	LSR_PE		0x04
#define 	LSR_FE		0x08
#define 	LSR_BI		0x10
#define 	LSR_THRE	0x20
#define 	LSR_TEMT	0x40
#define 	LSR_RXFE	0x80

//!< macros útiles para UARTs
#define		U0RDR		(U0LSR & LSR_RDR)
#define		U0THRE		((U0LSR & LSR_THRE) >>5)
#define		U1RDR		(U1LSR & LSR_RDR)
#define		U1THRE		((U1LSR & LSR_THRE) >>5)

/***********************************************************************************************************************************
 *** MACROS GLOBALES
 **********************************************************************************************************************************/

extern uint8_t in_tx ;
extern uint8_t out_tx ;

extern uint8_t cant_datosRx ;
extern uint8_t cant_datosTx;

extern uint8_t transmitiendo;

extern uint8_t out_rx ;
extern uint8_t in_rx ;

extern uint8_t bufferRx[TAM_BUFFERRX];
extern uint8_t bufferTx[TAM_BUFFERTX];

/***********************************************************************************************************************************
 *** TIPO DE DATOS GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
 **********************************************************************************************************************************/
void UART0_Inicializacion ();
uint8_t UART0_PushRX( uint8_t dato );
//uint8_t UART0_PopRX( uint8_t *dato );
//uint8_t UART0_PushTX( uint8_t dato );
uint8_t UART0_PopTX( uint8_t *dato );

void enviar(uint8_t dato);
#endif /* DRIVERLPC_DR_UART0_H_ */
