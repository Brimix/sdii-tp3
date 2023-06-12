/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2023, Brian Morris, Tomas Gonzalez
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "fsm_frameDetection.h"

/*==================[internal data definition]==========================*/
static fsm_frameDetectionState state;
static char frame[MAX_FRAME_SIZE];
static int size;

/*==================[internal functions definition]==========================*/
void handleFrame(char *frame, int size) {
	// printf("Frame received! Started processing...\n");
	bool isSuccessfulProcess = processFrame(frame, size);

	// printf("Processing finished. ");
	if (!isSuccessfulProcess) {
		// printf("Frame unrecognized\n");
		return;
	}

	//printf("Sending data...\n");
	uart_dma_envDatos(get_bufferEnv(), strlen((char*)get_bufferEnv()));
}

void clearFrame() {
	memset(frame, CHAR_END, sizeof(char) * MAX_FRAME_SIZE);
	size = 0;
}

/*==================[external functions definition]==========================*/
void fsm_frameDetection_init(){
	fsm_frameDetection_reset();
}

void fsm_frameDetection_reset(){
	state = AWAITING;
	clearFrame();
}

void fsm_frameDetection_execute() {
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
						// printf("Detected frame: %s\n", frame);
						handleFrame(frame, size);
						clearFrame();
						state = AWAITING;
						break;

					case BYTE_START:
						clearFrame();
						frame[size++] = byteReceived;
						break;

					default:
						if (size >= MAX_FRAME_SIZE) {
							// printf("Detected frame exceeded buffer size!\nDiscarding frame...");
							clearFrame();
							state = AWAITING;
						} else {
							frame[size++] = byteReceived;
						}
				}
			}
			break;
	}
}

/*==================[end of file]============================================*/