/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2023, Brian Morris, Tomas Gonzalez
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

#include "fsm_mode.h"

/*==================[internal data definition]==========================*/
static fsm_modeState state;

/*==================[internal functions definition]==========================*/
void sendMessageToOled(char* msg) {
	DISPLAY_OFF;
	DISPLAY_PUT(msg);
}

/*==================[external functions definition]==========================*/
void fsm_mode_init() {
	/* Initialize related peripherals */
	key_init();
	board_configSPI0();
	oled_init();
	oled_setContrast(16);


	/* Initialize FSM values */
	state = REQUEST_DATA;
	sendMessageToOled(REQUEST_DATA_MSG);
	fsm_frameDetection_init();
	fsm_sendAcceleration_init();
}

void fsm_mode_execute() {
	/* Getting value of switch by polling */
	bool shouldChangeState = GET_SW1;

	switch (state) {
		case REQUEST_DATA:
			fsm_frameDetection_execute();

			if (shouldChangeState) {
				fsm_sendAcceleration_reset();
				state = PLOT_3D;
				sendMessageToOled(PLOT_3D_MSG);
				// printf("Changed state to %d\n", state);
			}
			break;

		case PLOT_3D:
			fsm_sendAcceleration_execute();

			if (shouldChangeState) {
				fsm_frameDetection_reset();
				state = REQUEST_DATA;
				sendMessageToOled(REQUEST_DATA_MSG);
				// printf("Changed state to %d\n", state);
			}
			break;
	}
}

void fsm_mode_task1ms() {
	if (state == PLOT_3D) {
		fsm_sendAcceleration_task1ms();
	}
}
/*==================[end of file]============================================*/
