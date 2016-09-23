#include "USART_function.h"


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



// Send String via USART3
void USART_puts(char* s, USART_TypeDef* my_USART)
{
	while(*s){
		while(!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
		USART_SendData(USART3, *s++) ;
	}
}

// Send character via USART3
void USART_putc(uint16_t s, USART_TypeDef* my_USART)
{
	while(!USART_GetFlagStatus( USART3, USART_FLAG_TXE)) ;
	USART_SendData(USART3, s);
}

// Get character via USART3
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


void framming(char* cmd, USART_TypeDef* my_USART)
{
	uint8_t check_somme = STX;
	uint8_t check_somme_carry = 0;
	uint8_t counter = 1;
	
	check_somme_carry = 0;
	cmd[0] = check_somme = STX;
	USART_puts("PC command master (end with \'Q\'): ", my_USART) ;
	while((cmd[counter]=USART_receiver(my_USART)) != 'Q') {
		if(check_somme+cmd[counter] > 255) check_somme_carry = 1;
		check_somme += cmd[counter];
		counter++;
	}
	// We erase the last caracters 
	cmd[counter]=ETX;
	// Check somme final calcul and display
	check_somme = ~((check_somme+check_somme_carry+ETX));	

	USART_puts("\n\rCheck Somme: ", my_USART);
	USART_send_binary(check_somme, my_USART);
	USART_puts("\n\r", my_USART);

	USART_puts("END of transmission\n\r\n\r", my_USART);
	
}

void PC_to_Board(char* cmd, USART_TypeDef* my_USART)
{
	uint8_t check_somme = STX;
	uint8_t check_somme_carry = 0;
	uint8_t counter = 1;
	
	check_somme_carry = 0;
	cmd[0] = check_somme = STX;
	USART_puts("PC command master (end with \'Q\'): ", my_USART) ;
	while((cmd[counter]=USART_receiver(my_USART)) != 'Q') {
		if(check_somme+cmd[counter] > 255) check_somme_carry = 1;
		check_somme += cmd[counter];
		counter++;
	}
	// Check somme final calcul and display
	check_somme = ~((check_somme+check_somme_carry+ETX));	
	cmd[counter]=check_somme; // We replace la last character
	cmd[counter+1]=ETX;
	
	USART_puts("\n\rCheck Somme: ", my_USART);
	USART_send_binary(check_somme, my_USART);
	USART_puts("\n\r", my_USART);

	USART_puts("END of transmission\n\r\n\r", my_USART);
	
}





