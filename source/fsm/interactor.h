#ifndef INTERACTOR_H_
#define INTERACTOR_H_

#include <stdbool.h>
#include <math.h>

#include "pin_mux.h"
#include "MKL46Z4.h"
#include "efHal/uart_ringBuffer.h"
#include "bsp_kl46/SD2_board.h"
#include "bsp_kl46/mma8451.h"

typedef union {
	struct {
		char initChar;
		char teamCode[2];
		char actionType;
		char actionCode[2];
	};
	char data[6];
} FrameData_t;

uint8_t* get_bufferEnv();
bool isFrameRecieved();

void storeRecievedFrame();
void processFrame();

#endif
