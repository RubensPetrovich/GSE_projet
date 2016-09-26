#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32F4xx.h"
#include "STM32F4xx.h"

#include "USART_function.h"
#include "Handler.h"

struct MI {
	int j;
	char cmd_PC_board[cmd_length_max];
};

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


// main function
int main(void) {
	
	// Variable Initialisation
//	char cmd_PC_board[cmd_length_max]; // message send from the PC to the board
	NVIC_Configuration();
	
	// Initialisation of the GPIO and USART
  USART_Initialize() ;
	GPIO_Initialize() ;
	
	// Main while
	while(1)	{

	}	
}
