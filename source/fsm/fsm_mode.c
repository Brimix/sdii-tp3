#include "fsm_mode.h"

void fsm_mode_init() {
	oled_init();
	oled_setContrast(16);
	DISPLAY_OFF;
	DISPLAY_PUT(REQUEST_DATA_MSG);
}

void fsm_mode_execute() {
	static fsm_modeState state = REQUEST_DATA;

	bool shouldChangeState = GET_SW1;

	switch (state) {
		case REQUEST_DATA:
			fsm_frameDetection_execute();

			if (shouldChangeState) {
				// TODO: Config accel to send 1 interruption w/ measures
				state = PLOT_3D;
				DISPLAY_OFF;
				DISPLAY_PUT(PLOT_3D_MSG);
//				printf("Changed state to %d\n", state);
			}
			break;

		case PLOT_3D:
			fsm_sendAcceleration_execute();

			if (shouldChangeState) {
				// TODO: Reset accel config

				state = REQUEST_DATA;
				DISPLAY_OFF;
				DISPLAY_PUT(REQUEST_DATA_MSG);
//				printf("Changed state to %d\n", state);
			}
			break;
	}
}

void fsm_mode_task1ms() {
	fsm_sendAcceleration_task1ms();
}
