#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32F4xx.h"
#include "STM32F4xx.h"

#define cmd_length_max 64	// ATTENTION if more than 64, use for the comter uint16_t (or int) insted of uint8_t !!
#define STX 0x02
#define ETX 0x03
#define PC_TO_BOARD USART3
#define BOARD_TO_BOARD USART2

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
	
	// Definition USART 3 and 2
	USART_Init(USART3,&USARTT);
	USART_Init(USART2,&USARTT);
	
	// Enable USART 3 and 2
	USART_Cmd(USART3, ENABLE);	// Tx : PB10 -- Rx PB11
	USART_Cmd(USART2, ENABLE);	// Tx : PA02 -- Rx PA03
}

// Initialization GPIO
void GPIO_Initialize(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	
	// USART3 du GPIOB
	GPIO_PinAF (GPIOB, 10,GPIO_AF_USART3); 	// Tx
	GPIO_PinAF (GPIOB, 11,GPIO_AF_USART3);	// Rx
	
	// USART2 du GPIOA
	GPIO_PinAF (GPIOA, 2,GPIO_AF_USART2); // Tx
	GPIO_PinAF (GPIOA, 3,GPIO_AF_USART2);	// Rx
	
	//Configuration des pin
	GPIO_PinConfigure(GPIOB,10,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);
	GPIO_PinConfigure(GPIOB,11,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);
	GPIO_PinConfigure(GPIOA, 2,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);
	GPIO_PinConfigure(GPIOA, 3,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);
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

// get cmd from PC en return the string with ETX.. etc checkson...
void get_cmd_from_pc(char* cmd, USART_TypeDef* my_USART)
{
	uint8_t check_somme = STX;
	uint8_t check_somme_carry = 0;
	uint8_t counter = 1;
	
	check_somme_carry = 0;
	cmd[0] = check_somme = STX;
	USART_puts("PC command master (end with \'Q\'): ", my_USART) ;
	while((cmd[counter]=USART_receiver(my_USART)) != 0xD) {	// '\n'
		if(check_somme+cmd[counter] > 255) check_somme_carry = 1;
		check_somme += cmd[counter];
		counter++;
	}
	// Check somme final calcul and display
	check_somme = ~((check_somme+check_somme_carry+ETX));	
	cmd[counter]=check_somme; // We replace la last character
	cmd[++counter]=ETX;
	cmd[++counter]='\0';
		
	USART_puts("\n\rCheck Somme: ", my_USART);
	USART_send_binary(check_somme, my_USART);
	USART_puts("\n\r", my_USART);
	USART_puts("END of transmission\n\r\n\r", my_USART);

}


// main function
int main(void) {
	
	// Variable Initialisation
	char cmd_PC_board[cmd_length_max]; // message send from the PC to the board

	
	// Initialisation of the GPIO and USART
  USART_Initialize() ;
	GPIO_Initialize() ;
	
	// Main while
	while(1)	{

		get_cmd_from_pc(cmd_PC_board, PC_TO_BOARD);
		USART_puts(cmd_PC_board, BOARD_TO_BOARD);
		USART_puts("\n\rCommand Sent!\n\r", BOARD_TO_BOARD);
	}
}
