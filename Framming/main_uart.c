#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32F4xx.h"
#include "STM32F4xx.h"

#define cmd_length_max 64	// ATTENTION if more than 64, use for the comter uint16_t (or int) insted of uint8_t !!
#define STX 0x02
#define ETX 0x03

// Initialization USART 3
void USART3_Initialize(void) 
{
	USART_InitTypeDef USARTT ;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
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
}

// Initialization GPIO
void GPIO_Initialize(void)
{
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

// Send String via USART3
void USART3_puts(char* s)
{
	while(*s){
		while(!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
		USART_SendData( USART3, *s++) ;
	}
}

// Send character via USART3
void USART3_putc(uint16_t s)
{
	while(!USART_GetFlagStatus( USART3, USART_FLAG_TXE)) ;
	USART_SendData(USART3, s);
}

// Get character via USART3
uint16_t USART_receiver(void)
{
	while(!USART_GetFlagStatus( USART3, USART_FLAG_RXNE));
	return USART_ReceiveData(USART3) ;
}

// Send character in binary (8 bits) vuia USART3
void USART3_send_binary(uint16_t x)
{
	int i;
	char car;
	for(i=0 ; i<8 ; i++) {
		if (i == 4) USART3_putc(' ');
		car = (((x & 0x80) >> 7) == 0 ? '0':'1');
		USART3_puts(&car);
		x <<= 1;
	}
}

// main function
int main(void) {
	
	// Variable Initialisation
	char cmd_PC_board[cmd_length_max]; // message send from the PC to the board
	uint8_t check_somme = STX;
	uint8_t check_somme_carry = 0;
	uint8_t counter = 1;
	
	// Initialisation of the GPIO and USART
  USART3_Initialize() ;
	GPIO_Initialize() ;
	
	// Main while
	while(1)	{
		check_somme_carry = 0;
		cmd_PC_board[0] = check_somme = STX;
		USART3_puts("PC command master (end with \'Q\'): ") ;
		while((cmd_PC_board[counter]=USART_receiver()) != 'Q') {
			if(check_somme+cmd_PC_board[counter] > 255) check_somme_carry = 1;
			check_somme += cmd_PC_board[counter];
			counter++;
		}
		// We erase the last caracters 
		cmd_PC_board[counter]=ETX;
		// Check somme final calcul and display
		check_somme = ~((check_somme+check_somme_carry+ETX));	

		USART3_puts("\n\rCheck Somme: ");
		USART3_send_binary(check_somme);
		USART3_puts("\n\r");

		USART3_puts("END of transmission\n\r\n\r");
	}	
}
