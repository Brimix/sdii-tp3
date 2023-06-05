#include "handleFrame.h"

void handleFrame() {
	bool shouldProcess = isFrameRecieved();
	if (shouldProcess) {
		printf("Frame received! Started processing...\n");

		bool isSuccessfulProcess = processFrame();
		printf("Processing finished. ");
		if (isSuccessfulProcess) {
			printf("Sending data...\n");
			uart_dma_envDatos(get_bufferEnv(), strlen((char*)get_bufferEnv()));
		} else {
			printf("Frame unrecognized\n");
		}
	}
}
