#include <fsm/fsm_mode.h>
#include <stdio.h>
#include "board.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "bsp_kl46/SD2_board.h"
#include "bsp_kl46/mma8451.h"
#include "efHal/uart_ringBuffer.h"


int main(void){

	BOARD_BootClockRUN(); // Config del clock
	board_init(); // Config inicial de SW, LEDS, clock a los puertos, config de I2C, config mma8451
	mma8451_setDataRate(DR_12p5hz);

	uart_ringBuffer_init(); // Inicialización UART1, DMA, creación de Cola Circular
	board_configSPI0();

	fsm_mode_init();

	/* Inicializa interrupción de systick cada 1 ms */
	SysTick_Config(SystemCoreClock / 1000U);

    while(1)
    {
    	fsm_mode_execute();
    }
}

void SysTick_Handler(void) {
	fsm_mode_task1ms();
	key_periodicTask1ms();
}

