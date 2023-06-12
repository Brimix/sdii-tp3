#include "uart_ringBuffer.h"

/*==================[macros and definitions]=================================*/
#define UART1_TX_DMA_CHANNEL 0U
#define TX_BUFFER_DMA_SIZE  32

/*==================[internal data declaration]==============================*/
static uint8_t txBuffer_dma[TX_BUFFER_DMA_SIZE];
static uart_dma_handle_t UART1DmaHandle;
static dma_handle_t UART1TxDmaHandle;
volatile bool txOnGoing = false;
static void* pRingBufferRx;

/*==================[internal functions declaration]=========================*/

/* UART1 user callback */
static void UART_UserCallback(UART_Type *base, uart_dma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_UART_TxIdle == status)
    {
        txOnGoing = false;
    }
}

void uart_ringBuffer_init(void){
	uart_config_t config;

    pRingBufferRx = ringBuffer_init(16); //crea cola circualar de 16B


	UART_GetDefaultConfig(&config);//config UART1 para encapsular datos con la estructura 8N1
	config.baudRate_Bps = 115200;	//setea Baud Rate
	config.enableTx = true;	//habilita Tx
	config.enableRx = true;	//habilita Rx

	//Inicialización de la uart1 con la configuración establecida y el la frecuencia de clock del bus clock ( uart1 opera exclusivamente con bus clock)
	UART_Init(UART1, &config, CLOCK_GetFreq(kCLOCK_BusClk));

	/* Habilitacion de interrupciones */

    UART_EnableInterrupts(UART1, kUART_RxDataRegFullInterruptEnable); //Habilitación de interrupciones en la uart1 por buffer Rx lleno

    //Habilitación de interrupciones en la uart1 por OR, interrupción que será util en caso que esto ocurra para volver a habilitar Rx.
    UART_EnableInterrupts(UART1, kUART_RxOverrunInterruptEnable);

    NVIC_EnableIRQ(UART1_IRQn); // Habilitación de interrupción en NVIC por pedido de UART1

    uart_dma_init();

}

void uart_dma_init(void){
	/* CONFIGURACIÓN DMA (sólo para TX) */
	/* Init DMAMUX */
	DMAMUX_Init(DMAMUX0);

	/* Set channel for UART1  */
	DMAMUX_SetSource(DMAMUX0, UART1_TX_DMA_CHANNEL, kDmaRequestMux0UART1Tx);
	DMAMUX_EnableChannel(DMAMUX0, UART1_TX_DMA_CHANNEL);

	/* Init the DMA module */
	DMA_Init(DMA0);
	DMA_CreateHandle(&UART1TxDmaHandle, DMA0, UART1_TX_DMA_CHANNEL);

	/* Create UART1 DMA handle. */
	UART_TransferCreateHandleDMA(UART1,&UART1DmaHandle,UART_UserCallback,NULL,&UART1TxDmaHandle,NULL);
}



int32_t uart_ringBuffer_recDatos(uint8_t *pBuf, int32_t size)
{
    int32_t ret = 0;
    DisableIRQ(UART1_IRQn);//Deshabilita interupción por la UART1 momentáneamente para asegurar la integridad de los datos en la cola circular
    /* IMPORTANTE: Se deshabilita las interrupción por mma8451 (atenderlas demora bastante tiempo) evitando perder datos que nos llegan por UART1
       y además correr el riesgo de OverRun, deteniéndose la recepción de los datos*/
    DisableIRQ(PORTC_PORTD_IRQn);
    while (!ringBuffer_isEmpty(pRingBufferRx) && ret < size)
    {
        ringBuffer_getData(pRingBufferRx, &pBuf[ret]);
        ret++;
    }
    EnableIRQ(UART1_IRQn); //habilita interrupción del procesador por la UART1
    EnableIRQ(PORTC_PORTD_IRQn);//habilita nuevamente responder a interrupciones pedidas por el acelerómetro
    return ret;
}

int32_t uart_dma_envDatos(uint8_t *pBuf, int32_t size)
{
uart_transfer_t xfer;

    if (txOnGoing) {
        size = 0;
    } else {
        /* Limita size */
        if (size > TX_BUFFER_DMA_SIZE) size = TX_BUFFER_DMA_SIZE;

        // Hace copia del buffer a transmitir en otro arreglo para garantizar que el dato a transmitir es el deseado
        //(dado que podría haberse almacenado otro dato en la dirección de memoria que apunta pBuf)
        memcpy(txBuffer_dma, pBuf, size);

        xfer.data = txBuffer_dma;
        xfer.dataSize = size;

        board_Tx_Rx_485_Enable(Tx);//habilita transmision en el módulo RS485 y deshabilita recepcion

        /* Se inicia la transferencia del dato desde memoria hacia UART1 via DMA */
        txOnGoing = true;
        UART_TransferSendDMA(UART1, &UART1DmaHandle, &xfer);
        UART_EnableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable); //Se habilita interrupción en UART1 cuando la transferencia de datos este completa
    }
    return size;
}

void UART1_IRQHandler(void)
{
	uint8_t data;
    if ( (kUART_RxDataRegFullFlag)            & UART_GetStatusFlags(UART1) &&
         (kUART_RxDataRegFullInterruptEnable) & UART_GetEnabledInterrupts(UART1) )
	{
        /* obtiene dato recibido por puerto serie */
	    data = UART_ReadByte(UART1);
		/* pone dato en ring buffer */
		ringBuffer_putData(pRingBufferRx, data);
	}
    if ( (kUART_RxOverrunFlag)            & UART_GetStatusFlags(UART1) &&
         (kUART_RxOverrunInterruptEnable) & UART_GetEnabledInterrupts(UART1) )
	{
        /* obtiene dato recibido por puerto serie */
	    data = UART_ReadByte(UART1);
		/* pone dato en ring buffer */
		ringBuffer_putData(pRingBufferRx, data);
		/* Se limpia la bandera de overrun para que el Rx continue funcionando*/
		UART_ClearStatusFlags(UART1, kUART_RxOverrunFlag);
	}
	if ( (kUART_TransmissionCompleteFlag)            & UART_GetStatusFlags(UART1) &&
         (kUART_TransmissionCompleteInterruptEnable) & UART_GetEnabledInterrupts(UART1) )
	{
		board_Tx_Rx_485_Enable(Rx);//Se habilita la recepción en el módulo RS485 (se deshabilita la transmisión, no interesa escuchar lo que se envía)
        UART_DisableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable);
        UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag);
	}
}

/*==================[end of file]============================================*/
