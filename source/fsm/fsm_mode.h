#ifndef FSM_FRAMEDETECTION_H_
#define FSM_FRAMEDETECTION_H_

#include "bsp_kl46/key.h"
#include "fsm_frameDetection.h"

#define GET_SW1 key_getPressEv(BOARD_SW_ID_1);

typedef enum {
	REQUEST_DATA = 0,
	PLOT_3D,
} fsm_modeState;

void fsm_mode_execute();

#endif
