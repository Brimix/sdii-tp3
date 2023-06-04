/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
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

#ifndef SD2_BOARD_H_
#define SD2_BOARD_H_

/*==================[inclusions]=============================================*/
#include "MKL46Z4.h"
#include <stdbool.h>
#include "SD2_I2C.h"
#include "mma8451.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

typedef enum
{
    BOARD_LED_ID_ROJO = 0,
    BOARD_LED_ID_VERDE,
    BOARD_LED_ID_TOTAL

}board_ledId_enum;

typedef enum
{
    Rx,
	Tx
}board_RxTx_enum;

typedef enum
{
    BOARD_SW_ID_1 = 0,
    BOARD_SW_ID_3,
    BOARD_SW_ID_TOTAL
}board_swId_enum;

typedef enum
{
    BOARD_LED_MSG_OFF = 0,
    BOARD_LED_MSG_ON,
    BOARD_LED_MSG_TOGGLE
}board_ledMsg_enum;

typedef struct
{
    PORT_Type *port;
    GPIO_Type *gpio;
    uint32_t pin;
}board_gpioInfo_type;

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/
#define SOPT5_UART1RXSRC_UART_RX 0x00u /*!<@brief UART1 Receive Data Source Select: UART1_RX pin */
#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART1 Transmit Data Source Select: UART1_TX pin */
/*==================[external functions definition]==========================*/

/** \brief inicializaci�n del hardware
 **
 **/
void board_init(void);

/** \brief setea estado del led indicado
 **
 **/

void board_Tx_Rx_485_Enable(board_RxTx_enum dato);

void board_setLed(board_ledId_enum id, board_ledMsg_enum msg);

/** \brief Devuelve estado del pulsador indicado
 **
 ** \return true: si el pulsdor est� apretado
 **         false: si el pulsador no est� apretado
 **/
bool board_getSw(board_swId_enum id);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* SD2_BOARD_H_ */
