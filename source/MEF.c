#include <MEF.h>
#include <string.h>
static char *buffer_proc;
uint8_t bufferEnv[32];
static bool flag_proc=0,flag_responder=0;

static char trama[MAX_REC];

void mef_detectar_trama(){
    static mef_detectar_trama_enum estado_detectar_trama = MEF_DETECTAR_ESPERANDO;
    uint32_t T_det;
    uint8_t B_rec;
    static uint8_t indicetrama = 0;
    T_det = uart_ringBuffer_recDatos(&B_rec, sizeof(B_rec));
    switch (estado_detectar_trama){

    	case MEF_DETECTAR_ESPERANDO:
    	if(T_det != 0){
    		if (B_rec == ':'){
    			trama[indicetrama] = B_rec;
    			indicetrama++;
    			estado_detectar_trama = MEF_DETECTAR_1;
    		}}
    	break;
    	case MEF_DETECTAR_1:
    	if(T_det != 0){
    		if (B_rec == '1'){
    			trama[indicetrama] = B_rec;
    			indicetrama++;
    			estado_detectar_trama = MEF_DETECTAR_6;}
    		else if (B_rec != '1'){
    			indicetrama=0;
    			estado_detectar_trama = MEF_DETECTAR_ESPERANDO;
    		}}
    	break;

    	case MEF_DETECTAR_6:
    	if(T_det != 0){
    		if (B_rec == '6'){
    			trama[indicetrama] = B_rec;
    			indicetrama++;
    			estado_detectar_trama = MEF_DETECTAR_RESTO;
    	  	}
    		else if (B_rec != '6'){
    			indicetrama=0;
    			estado_detectar_trama = MEF_DETECTAR_ESPERANDO;
    		}}
    	break;
    	case MEF_DETECTAR_RESTO:
    	if(indicetrama>=MAX_REC){ // Se excedió la cantidad de bytes recibidos, por lo que dejamos de recibir
    		indicetrama = 0;
    		estado_detectar_trama = MEF_DETECTAR_ESPERANDO;}
    	if(T_det!=0){
    		if(B_rec == ':'){
    			indicetrama = 0;
    			trama[indicetrama] = B_rec;
    			indicetrama++;
    			estado_detectar_trama = MEF_DETECTAR_1;
    		}
    		else if(B_rec != CHAR_FIN){
    			trama[indicetrama] = B_rec;
    			indicetrama++;
    		}
    		else if (B_rec == CHAR_FIN){
    			procesar_trama(trama,indicetrama);
    			indicetrama = 0;
    			estado_detectar_trama = MEF_DETECTAR_ESPERANDO;
    		}}
    	break;
	}
}

void LEDS(){ //cuando se detecta accion sobre LED
	switch(buffer_proc[4]){
	case '1':
		switch(buffer_proc[5]){
			case 'E':
			board_setLed(BOARD_LED_ID_ROJO,BOARD_LED_MSG_ON);
			flag_responder=1;
			break;

			case 'A':
			board_setLed(BOARD_LED_ID_ROJO,BOARD_LED_MSG_OFF);
			flag_responder=1;
			break;

			case 'T':
			board_setLed(BOARD_LED_ID_ROJO,BOARD_LED_MSG_TOGGLE);
			flag_responder=1;
			break;
		}
		break;

		case '2':
		switch(buffer_proc[5])
		{
			case 'E':
			board_setLed(BOARD_LED_ID_VERDE,BOARD_LED_MSG_ON);
			flag_responder=1;
			break;

			case 'A':
			board_setLed(BOARD_LED_ID_VERDE,BOARD_LED_MSG_OFF);
			flag_responder=1;
			break;

			case 'T':
			board_setLed(BOARD_LED_ID_VERDE,BOARD_LED_MSG_TOGGLE);
			flag_responder=1;
			break;
		}
	}
}

void SWITCH(){ //cuando se detecta lectura SW
	switch(buffer_proc[4])
	{
		case '1':
		if(board_getSw(BOARD_SW_ID_1))
		{
			sprintf((char*)bufferEnv, "%sP\n",buffer_proc);
			flag_responder=1;
		}
		else
		{
			sprintf((char*)bufferEnv, "%sN\n",buffer_proc);
			flag_responder=1;
		}
		break;

		case '3':
		if(board_getSw(BOARD_SW_ID_3))
		{
			sprintf((char*)bufferEnv, "%sP\n",buffer_proc);
			flag_responder=1;
		}
		else
		{
			sprintf((char*)bufferEnv, "%sN\n",buffer_proc);
			flag_responder=1;
		}
		break;
	}
}

void ACELEROM(){ //cuando se detecta medicion de la aceleracion

	int16_t acc;
	acc=sqrt(pow(mma8451_getAcX(),2)+pow(mma8451_getAcY(),2)+pow(mma8451_getAcZ(),2));
	sprintf((char*)bufferEnv, "%s%d\n",buffer_proc,acc);
}

void Procesando(){ //ejecuta función segun periferico que deba intervenir

	switch(buffer_proc[3]){

	case '0':
		LEDS();
		sprintf((char*)bufferEnv, "%s\n",buffer_proc);
	break;

	case '1':
		SWITCH();
	break;

	case '2':
		if(buffer_proc[4] == '0')
		{
			ACELEROM();
			flag_responder = 1;
		}
	break;
	}
}

void mef_procesar_trama()
{
	static mef_procesar_trama_enum estado = MEF_PROCESAR_ESPERANDO;

	switch(estado)
	{
	case MEF_PROCESAR_ESPERANDO:
		if(flag_proc==1){
			estado = MEF_PROCESAR_PROCESANDO;
		}
	break;

	case MEF_PROCESAR_PROCESANDO:
	Procesando();
	flag_proc=0;
		if(flag_proc==0){
			//Envia rsta por uart1 a la pc. Trae por DMA los datos de memoria al buffer Tx
			if(flag_responder==1){
				uart_dma_envDatos(bufferEnv, strlen((char*)bufferEnv));
				flag_responder=0;
			}
			estado = MEF_PROCESAR_ESPERANDO;
		}
	break;
	}
}

void procesar_trama(char *bufferRec,int longitud){
	//guarda en buffer_proc la transmision recibida y
	//setea flag_proc para iniciar el procesamiento del msj recibido

	flag_proc = 1;
	buffer_proc = bufferRec;
	buffer_proc[longitud] = '\0';
}
