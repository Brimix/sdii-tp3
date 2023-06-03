#include <MEF.h>
#include <stdio.h>
#include "board.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "uart_ringBuffer.h"


int main(void){

	BOARD_BootClockRUN();//Config del clock
	board_init(); //Config inicial de SW, LEDS, clock a los puertos, config de I2C, config mma8451
	mma8451_setDataRate(DR_12p5hz);
	uart_ringBuffer_init(); // Inicialización UART1, DMA, creación de Cola Circular
    while(1)
    {
    	mef_detectar_trama();
    	mef_procesar_trama();
    }
}

