#include "fsm_frameDetection.h"

const char* TEAM_ID = ":16";

void receiveFrame(char* frame, int size) {
	bool matchesMyPattern = (size >= strlen(TEAM_ID)) && strncmp(TEAM_ID, frame, strlen(TEAM_ID))==0;
	if (!matchesMyPattern) {
		//printf("Frame is not for me!\nDiscarding frame...\n");
	}else{
	storeReceivedFrame(frame, size);
	fsm_frameDetection_handle();
	}
}

void fsm_frameDetection_execute() {
    static fsm_frameDetectionState state = AWAITING;
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
						receiveFrame(frame, size);
						size = 0;
						state = AWAITING;



						break;

					case BYTE_START:
						size = 0;
						frame[size++] = byteReceived;
						break;

					default:
						if (size >= MAX_FRAME_SIZE) { // Se excedió la cantidad de bytes recibidos, por lo que dejamos de recibir
							//printf("Detected frame exceeded buffer size!\nDiscarding frame...");
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


void fsm_frameDetection_handle() {
	bool shouldProcess = isFrameRecieved();
	if (shouldProcess) {
		//printf("Frame received! Started processing...\n");

		bool isSuccessfulProcess = processFrame();
		//printf("Processing finished. ");
		if (isSuccessfulProcess) {
			//printf("Sending data...\n");
			uart_dma_envDatos(get_bufferEnv(), strlen((char*)get_bufferEnv()));
		} else {
			printf("Frame unrecognized\n");
		}
	}
}
