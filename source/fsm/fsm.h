#ifndef FSM_H_
#define FSM_H_

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "pin_mux.h"
#include "MKL46Z4.h"
#include "efHal/uart_ringBuffer.h"
#include "bsp_kl46/SD2_board.h"
#include "bsp_kl46/mma8451.h"


typedef enum{
	NO,
	SI
}HABILITADO_enum;

typedef enum{
	MEF_PROCESAR_ESPERANDO = 0,
	MEF_PROCESAR_PROCESANDO,
}mef_procesar_trama_enum;


void LEDS();
void SWITCH();
void ACELEROM();
void Procesando();
void procesar_trama(char *buffer, int longitud);
void mef_procesar_trama();

#endif
