/*
 * IMPORTANT!
 *
 * This module should be renamed and moved out of this folder.
 * And recoded, please...
 */
#include "interactor.h"

uint8_t bufferEnv[32];
FrameData_t receivedFrame;
bool frameRecieved = false, responseReady = false;

uint8_t* get_bufferEnv() { return bufferEnv; }

bool isFrameRecieved() {
	if (frameRecieved) {
		frameRecieved = false; // Clearing flag
		return true;
	}
	return false;
}

void storeReceivedFrame(char *bufferRec, int length) {
	frameRecieved = true;
	strcpy(receivedFrame.data, bufferRec);
	receivedFrame.data[length] = '\0';
}

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

bool processFrame() { //ejecuta función segun periferico que deba intervenir
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
