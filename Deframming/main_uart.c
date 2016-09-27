#include "commun.h"
#include "USART_function.h"
#include "Handler.h"
#include "LED.h"

char commande[cmd_length_max];
uint8_t command_sent;


// Initialization GPIO
void GPIO_Initialize(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	
	// USART3 du GPIOB
	GPIO_PinAF (GPIOB, 10,GPIO_AF_USART3); 	// Tx
	GPIO_PinAF (GPIOB, 11,GPIO_AF_USART3);	// Rx
	
	
	//Configuration des pin
	GPIO_PinConfigure(GPIOB,10,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);
	GPIO_PinConfigure(GPIOB,11,GPIO_MODE_AF, GPIO_OUTPUT_PUSH_PULL, GPIO_OUTPUT_SPEED_100MHz, GPIO_PULL_UP);

}


// main function
int main(void) {
	
	// Variable Initialisation
//	char commande[cmd_length_max];
	LED_Initialize();
	//	char cmd_PC_board[cmd_length_max]; // message send from the PC to the board
	NVIC_Configuration();
	
	// Initialisation of the GPIO and USART
  
	GPIO_Initialize() ;
	USART_Initialize() ;
	
	command_sent = 0;
	
	// Main while
	while(1)	{
		USART_puts("Hey ! ", USART3);
		while(!command_sent);
		if (commande[0] == '1') {
			LED_On(0);
			LED_Off(1);
			LED_Off(2);
			LED_Off(3);
		}
		if (commande[0] == '2') {
			LED_Off(0);
			LED_On(1);
			LED_Off(2);
			LED_Off(3);
		}
		if (commande[0] == '3') {
			LED_Off(0);
			LED_Off(1);
			LED_On(2);
			LED_Off(3);
		}
		if (commande[0] == '4') {
			LED_Off(0);
			LED_Off(1);
			LED_Off(2);
			LED_On(3);
		}
		command_sent = 0;
	}
}
