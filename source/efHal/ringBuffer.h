/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright, Diego Alegrechi
 * Copyright, Gustavo Muro
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

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

/*==================[inclusions]=============================================*/
#include "stdint.h"
#include "stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef struct
{
    int32_t indexRead;
    int32_t indexWrite;
    int32_t count;
    int32_t size;
    uint8_t *pBuf;
} ringBuferData_struct;

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

/** \brief inicializa buffer circular de bytes
 **
 ** \param[in] size tamaño del buffer
 **
 ** \return puntero a buffer circular, NULL si no se pudo crear
 **/
void *ringBuffer_init(int32_t size);

/** \brief libera de memoria buffer circular
 **
 ** \param[in] pRb puntero al buffer circular
 **
 **/
void ringBuffer_deInit(ringBuferData_struct *pRb);

/** \brief pone nuevo dato en el buffer circular
 ** si el buffer esta lleno se pisa el dato más antiguo
 ** y se avanza el puntero de lectura
 **
 ** \param[inout] pRb puntero al buffer circular
 ** \param[in] data dato a colocar en el buffer
 **
 ** \return true si se pudo poner el dato correctamente
 **         false si se puso y se piso un dato antiguo
 **/
bool ringBuffer_putData(void *pRb, uint8_t data);

/** \brief devuelve dato más antiguo del buffer
 **
 ** \param[inout] pRb puntero al buffer circular
 ** \param[in] data puntero a donde guardar el dato
 **
 ** \return true si se extrajo el dato del buffer
 **         false si no había datos en el buffer
 **/
bool ringBuffer_getData(void *pRb, uint8_t *data);

/* TODO: agregar comentarios */
bool ringBuffer_isFull(void *pRb);

/* TODO: agregar comentarios */
bool ringBuffer_isEmpty(void *pRb);



/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* RING_BUFFER_H_ */
