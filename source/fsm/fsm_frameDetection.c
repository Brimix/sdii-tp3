#include "fsm_frameDetection.h"

static fsm_frameDetectionState state = AWAITING;

void handleFrame(char *frame, int size) {
	// printf("Frame received! Started processing...\n");

	bool matchesMyPattern = (size >= strlen(TEAM_ID)) && strncmp(TEAM_ID, frame, strlen(TEAM_ID))==0;
	if (!matchesMyPattern) {
		printf("Frame is not for me!\nDiscarding frame... %s\n", frame);
		return;
	}

	bool isSuccessfulProcess = processFrame(frame, size);
	// printf("Processing finished. ");
	if (!isSuccessfulProcess) {
//		printf("Frame unrecognized\n");
		return;
	}

	//printf("Sending data...\n");
	uart_dma_envDatos(get_bufferEnv(), strlen((char*)get_bufferEnv()));
}
void fsm_frameDetection_init(){
	fsm_frameDetection_reset();
}

void fsm_frameDetection_reset(){
	state = AWAITING;
}


void fsm_frameDetection_execute() {

    static char frame[MAX_FRAME_SIZE];
    static int size = 0;

    uint8_t byteReceived;
    uint32_t bytesReceivedCount = uart_ringBuffer_recDatos(&byteReceived, sizeof(byteReceived));

    switch (state) {
    	case AWAITING:
    		if (bytesReceivedCount && byteReceived == BYTE_START) {
    			frame[size++] = byteReceived;
				state = RECEIVING;
    		}
    		break;

    	case RECEIVING:
			if (bytesReceivedCount) {
				switch (byteReceived) {
					case BYTE_FIN:
						//printf("Detected frame: %s\n", frame);
						handleFrame(frame, size);
						memset(frame, CHAR_END, sizeof(char) * MAX_FRAME_SIZE);
						size = 0;
						state = AWAITING;
						break;

					case BYTE_START:
						memset(frame, CHAR_END, sizeof(char) * MAX_FRAME_SIZE);
						size = 0;
						frame[size++] = byteReceived;
						break;

					default:
						if (size >= MAX_FRAME_SIZE) {
							//printf("Detected frame exceeded buffer size!\nDiscarding frame...");
							memset(frame, CHAR_END, sizeof(char) * MAX_FRAME_SIZE);
							size = 0;
							state = AWAITING;
						} else {
							frame[size++] = byteReceived;
						}
				}
			}
			break;
	}
}
