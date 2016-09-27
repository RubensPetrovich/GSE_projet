#include "USART_function.h"

extern char commande[cmd_length_max];
extern uint8_t command_sent;


// Initialization USART 3 and 2
void USART_Initialize(void) 
{
	USART_InitTypeDef USARTT ;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	// Usart properties
	USARTT.USART_BaudRate = 9600 ;
	USARTT.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	USARTT.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;
	USARTT.USART_Parity = USART_Parity_No ; 
	USARTT.USART_StopBits = USART_StopBits_1 ;
	USARTT.USART_WordLength = USART_WordLength_8b ;
	
	// Definition USART 3
	USART_Init(USART3,&USARTT);
	
	// Enable USART 3
	USART_Cmd(USART3, ENABLE);	// Tx : PB10 -- Rx PB11
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


// Send character via USART3 or USAR2
void USART_putc(uint16_t s, USART_TypeDef* my_USART)
{
	while(!USART_GetFlagStatus( my_USART, USART_FLAG_TXE)) ;
	USART_SendData(my_USART, s);
}

// Send String via USART3 or USAR2
void USART_puts(char* s, USART_TypeDef* my_USART)
{
	while(*s){
		USART_putc(*s++, my_USART);
	}
}

// Get character via USART3 or USAR2
uint16_t USART_receiver(USART_TypeDef* my_USART)
{
	while(!USART_GetFlagStatus( USART3, USART_FLAG_RXNE));
	return USART_ReceiveData(USART3) ;
}

// Send character in binary (8 bits) vuia USART3
void USART_send_binary(uint16_t x, USART_TypeDef* my_USART)
{
	int i;
	char car;
	for(i=0 ; i<8 ; i++) {
		if (i == 4) USART_putc(' ', my_USART);
		car = (((x & 0x80) >> 7) == 0 ? '0':'1');
		USART_puts(&car, my_USART);
		x <<= 1;
	}
}


