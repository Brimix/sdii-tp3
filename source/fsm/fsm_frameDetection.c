#include <MKL46Z4.h>
#include "fsm_frameDetection.h"
#include "fsm.h"

static char frame[MAX_FRAME_SIZE];

void fsm_frameDetection_execute() {
    static fsm_frameDetectionState state = MEF_DETECTAR_ESPERANDO;

    // TOOO: Change variable name once we understand exactly how it works
    uint32_t isFrameDetected;
    uint8_t recievedByte;
    static uint8_t index = 0;

    isFrameDetected = uart_ringBuffer_recDatos(&recievedByte, sizeof(recievedByte));

    switch (state) {
    	case MEF_DETECTAR_ESPERANDO:
    		if(isFrameDetected != 0) {
    			if (recievedByte == ':') {
    				frame[index] = recievedByte;
    				index++;
    				state = MEF_DETECTAR_1;
    			}
    		}
    		break;
    	case MEF_DETECTAR_1:
			if(isFrameDetected != 0){
				if (recievedByte == '1'){
					frame[index] = recievedByte;
					index++;
					state = MEF_DETECTAR_6;}
				else if (recievedByte != '1'){
					index=0;
					state = MEF_DETECTAR_ESPERANDO;
				}
			}
			break;
    	case MEF_DETECTAR_6:
    		if(isFrameDetected != 0) {
    			if (recievedByte == '6') {
    				frame[index] = recievedByte;
    				index++;
    				state = MEF_DETECTAR_RESTO;
    			}
				else if (recievedByte != '6') {
					index = 0;
					state = MEF_DETECTAR_ESPERANDO;
				}
    		}
    		break;
    	case MEF_DETECTAR_RESTO:
			if (index >= MAX_FRAME_SIZE) { // Se excedió la cantidad de bytes recibidos, por lo que dejamos de recibir
				index = 0;
				state = MEF_DETECTAR_ESPERANDO;
			}
			if (isFrameDetected != 0) {
				if (recievedByte == ':') {
					index = 0;
					frame[index] = recievedByte;
					index++;
					state = MEF_DETECTAR_1;
				}
				else if(recievedByte != BYTE_FIN) {
					frame[index] = recievedByte;
					index++;
				}
				else if (recievedByte == BYTE_FIN) {
					procesar_trama(frame,index);
					index = 0;
					state = MEF_DETECTAR_ESPERANDO;
				}
			}
			break;
	}
}
