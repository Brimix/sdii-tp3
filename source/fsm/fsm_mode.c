#include "fsm_mode.h"

void fsm_mode_execute() {
	static fsm_modeState state = REQUEST_DATA;

	bool shouldChangeState = GET_SW1;

	switch (state) {
		case REQUEST_DATA:
			fsm_frameDetection_execute();

			if (shouldChangeState) {
				// do stuff...
				state = PLOT_3D;
//				printf("Changed state to %d\n", state);
			}
			break;

		case PLOT_3D:
			// TODO: Implement new FSM for 3d plotting
			if (shouldChangeState) {
				// do stuff...
				state = REQUEST_DATA;
//				printf("Changed state to %d\n", state);
			}
			break;
	}
}
