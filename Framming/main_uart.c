#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32F4xx.h"
#include "STM32F4xx.h"

void USART3_Initialize(void) {
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
	
	// Definition USART 2 & 3
	USART_Init(USART3,&USARTT);
	USART_Init(USART2,&USARTT);
	
	// Enable USART 2 & 3
	USART_Cmd(USART3, ENABLE);	// Tx : PB10 -- Rx PB11
	USART_Cmd(USART2, ENABLE);	// Tx : PA2  -- Rx PA3
	
}

void GPIO_Initialize(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	
	//Pour activer le mode alternate function / USART3 du GPIOB pour Tx
	GPIO_PinAF (GPIOB, 10,GPIO_AF_USART3); 
	GPIO_PinAF (GPIOB, 11,GPIO_AF_USART3);
	//Configuration du Pin 10
	GPIO_PinConfigure(GPIOB,10,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);
	//Pour activer le mode alternate function / USART3 du GPIOB pour Rx
	//Configuration du Pin 11
	GPIO_PinConfigure(GPIOB,11,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);
}


int USART3_transmitter_empty(void){
		return USART_GetFlagStatus( USART3, USART_FLAG_TXE) ;
}
	
void USART3_puts( char *s) {
	while(*s){
		USART_SendData( USART3, *s++) ;
		while(USART3_transmitter_empty() == 0) ;
	}
}

int USART3_receiver_ready(void) {
	return USART_GetFlagStatus( USART3, USART_FLAG_RXNE) ;
}

uint16_t USART_receiver(void) {
	uint16_t a ;
	while(!USART_GetFlagStatus( USART3, USART_FLAG_RXNE));
	a = USART_ReceiveData(USART3) ;
	USART_SendData(USART3, a);
	while(USART3_transmitter_empty() == 0) ;
	USART_SendData(USART3, a);
	while(USART3_transmitter_empty() == 0) ;
	USART3_puts(" OK ");
	
	return a;
}

int main(void) {
	uint16_t a;
	char *s = "HELLO WORLD : ";
  USART3_Initialize() ;
	GPIO_Initialize() ;
	
	USART3_puts(s) ;
	USART3_puts("TEST");
	
	while(USART3_transmitter_empty() == 0) ;

	USART_SendData( USART3, '!') ;
	while(1)	{
		a = USART_receiver();
	}
	
	
	//USART_receiver() ;
}
