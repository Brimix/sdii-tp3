/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2023, Brian Morris
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

#include "fsm_sendAcceleration.h"

/*==================[internal data declaration]==============================*/

int timerDelay = 0;

/*==================[internal functions declaration]=========================*/

void sendAcceleration() {
	printf("Ready to send acceleration!\n");
	uint8_t sendMessage[15];

	int16_t accX,accY,accZ;
	accX = mma8451_getAcX();
	accY = mma8451_getAcY();
	accZ = mma8451_getAcZ();

	sprintf((char*)sendMessage, "%d %d %d\0", accX, accY, accZ);
	printf("Enviando mensaje: %s\n", sendMessage);
	uart_dma_envDatos(sendMessage, strlen((char*)sendMessage));
}

/*==================[external functions declaration]=========================*/
void fsm_sendAcceleration_execute() {
	static fsm_sendAccelerationState state = SEND_DATA;
	bool isDataReady = false;

	switch (state) {
		case SEND_DATA:
			isDataReady = mma8451_isDataReady();

			if (isDataReady) {
				sendAcceleration();

				// TODO: Request another measure of accel
				state = WAIT_DELAY;
				timerDelay = SEND_DELAY;
			}
			break;

		case WAIT_DELAY:
			if (timerDelay <= 0) {
				state = SEND_DATA;
			}
			break;
	}
}

void fsm_sendAcceleration_task1ms() {
	if (timerDelay > 0) timerDelay--;
}
