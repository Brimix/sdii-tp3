/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2023, Brian Morris / Tomas Gonzalez
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <board.h>
#include <clock_config.h>
#include <MKL46Z4.h>

#include "bsp_kl46/SD2_board.h"
#include "bsp_kl46/mma8451.h"
#include "efHal/uart_ringBuffer.h"
#include "fsm/fsm_mode.h"

int main(void){
	/* Config del clock */
	BOARD_BootClockRUN(); 

	/* Config inicial de SW, LEDS, clock a los puertos, config de I2C, config mma8451 */
	board_init(); 

	/* Inicialización UART1, DMA, creación de Cola Circular */
	uart_ringBuffer_init();

	/* Inicialización de FEM global */
	fsm_mode_init();

	/* Inicializa interrupción de systick cada 1 ms */
	SysTick_Config(SystemCoreClock / 1000U);

    while(1)
    {
    	fsm_mode_execute();
    }
}

void SysTick_Handler(void) {
	key_periodicTask1ms();
	fsm_mode_task1ms();
}

