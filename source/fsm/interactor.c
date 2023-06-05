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

void storeRecievedFrame(char *bufferRec, int length) {
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
	sprintf((char*)bufferEnv, "%s\n",receivedFrame);
}

void SWITCH() { //cuando se detecta lectura SW
	switch (receivedFrame.actionCode[0]) {
		case '1':
			if (board_getSw(BOARD_SW_ID_1)) {
				sprintf((char*)bufferEnv, "%sP\n",receivedFrame);
				responseReady=1;
			} else {
				sprintf((char*)bufferEnv, "%sN\n",receivedFrame);
				responseReady=1;
			}
			break;

		case '3':
			if (board_getSw(BOARD_SW_ID_3)) {
				sprintf((char*)bufferEnv, "%sP\n",receivedFrame);
				responseReady=1;
			} else {
				sprintf((char*)bufferEnv, "%sN\n",receivedFrame);
				responseReady=1;
			}
			break;
	}
}

void ACELEROM() { //cuando se detecta medicion de la aceleracion
	if(receivedFrame.actionCode[0] == '0') {
		int16_t acc;
		acc = sqrt(pow(mma8451_getAcX(),2)+pow(mma8451_getAcY(),2)+pow(mma8451_getAcZ(),2));
		sprintf((char*)bufferEnv, "%s%d\n",receivedFrame,acc);
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
