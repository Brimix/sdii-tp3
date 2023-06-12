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

#include "frameProcessor.h"

/*==================[internal data definition]==========================*/
uint8_t bufferEnv[32];
FrameData_t receivedFrame;
bool responseReady = false;

void LEDS() { //cuando se detecta accion sobre LED
	switch (receivedFrame.actionCode[0]) {
		case '1':
			switch(receivedFrame.actionCode[1]) {
				case 'E':
					board_setLed(BOARD_LED_ID_ROJO,BOARD_LED_MSG_ON);
					responseReady = 1;
					break;

				case 'A':
					board_setLed(BOARD_LED_ID_ROJO,BOARD_LED_MSG_OFF);
					responseReady = 1;
					break;

				case 'T':
					board_setLed(BOARD_LED_ID_ROJO,BOARD_LED_MSG_TOGGLE);
					responseReady = 1;
					break;
			}
			break;

		case '2':
			switch (receivedFrame.actionCode[1]) {
				case 'E':
					board_setLed(BOARD_LED_ID_VERDE,BOARD_LED_MSG_ON);
					responseReady=1;
					break;

				case 'A':
					board_setLed(BOARD_LED_ID_VERDE,BOARD_LED_MSG_OFF);
					responseReady=1;
					break;

				case 'T':
					board_setLed(BOARD_LED_ID_VERDE,BOARD_LED_MSG_TOGGLE);
					responseReady=1;
					break;
			}
			break;
	}
	sprintf((char*)bufferEnv, "%s\n",receivedFrame.data);
}

void SWITCH() { //cuando se detecta lectura SW
	switch (receivedFrame.actionCode[0]) {
		case '1':
			if (board_getSw(BOARD_SW_ID_1)) {
				sprintf((char*)bufferEnv, "%sP\n",receivedFrame.data);
				responseReady=1;
			} else {
				sprintf((char*)bufferEnv, "%sN\n",receivedFrame.data);
				responseReady=1;
			}
			break;

		case '3':
			if (board_getSw(BOARD_SW_ID_3)) {
				sprintf((char*)bufferEnv, "%sP\n",receivedFrame.data);
				responseReady=1;
			} else {
				sprintf((char*)bufferEnv, "%sN\n",receivedFrame.data);
				responseReady=1;
			}
			break;
	}
}

void ACELEROM() { //cuando se detecta medicion de la aceleracion
	if(receivedFrame.actionCode[0] == '1') {

		int32_t accX,accY,accZ;
		accX= mma8451_getAcX();
		accY= mma8451_getAcY();
		accZ= mma8451_getAcZ();


		sprintf((char*)bufferEnv, "%s%+04d%+04d%+04d\n",receivedFrame.data,(int)accX,(int)accY,(int)accZ);
		responseReady = 1;
	}
}

/*==================[external functions definition]==========================*/
uint8_t* get_bufferEnv() { return bufferEnv; }

bool processFrame(char *bufferRec, int length) { //ejecuta función segun periferico que deba intervenir
	if (length < MIN_EXCPECTED_LENGTH) {
		// printf("Frame too short\nDiscarding frame... %s\n", bufferRec);
		return false;
	}

	strcpy(receivedFrame.data, bufferRec);
	receivedFrame.data[length] = '\0';

	bool matchesMyPattern = strcmp(TEAM_ID, receivedFrame.teamCode) == 0;
	if (!matchesMyPattern) {
		// printf("Frame is not for me!\nDiscarding frame... %s\n", bufferRec);
		return false;
	}

	switch (receivedFrame.actionType) {
		case '0':
			LEDS();
			break;

		case '1':
			SWITCH();
			break;

		case '2':
			ACELEROM();
			break;
	}
	return responseReady;
}

/*==================[end of file]============================================*/