
#ifndef UART_RING_BUFFER_H_
#define UART_RING_BUFFER_H_

/*==================[inclusions]=============================================*/

// Standard C Included Files
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "stdint.h"
// Project Included Files
#include "SD2_board.h"
#include "fsl_uart.h"
#include "MKL46Z4.h"
#include "ringBuffer.h"
#include "fsl_uart_dma.h"
#include "fsl_dmamux.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

void uart_ringBuffer_init(void);
int32_t uart_ringBuffer_recDatos(uint8_t *pBuf, int32_t size);
int32_t uart_dma_envDatos(uint8_t *pBuf, int32_t size);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* UART_RING_BUFFER_H_ */
