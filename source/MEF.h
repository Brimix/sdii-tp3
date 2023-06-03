#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pin_mux.h"
#include "MKL46Z4.h"
#include "uart_ringBuffer.h"
#include "SD2_board.h"
#include "mma8451.h"


#define MAX_REC		8
#define CHAR_FIN	0x0A

typedef enum{
	MEF_DETECTAR_ESPERANDO = 0,
	MEF_DETECTAR_1,
	MEF_DETECTAR_6,
	MEF_DETECTAR_RESTO,
}mef_detectar_trama_enum;

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
void mef_detectar_trama();


