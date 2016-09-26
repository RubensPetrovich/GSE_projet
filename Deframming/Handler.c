
#include "Handler.h"
#include "USART_function.h"


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
  NVIC_Init(&NVIC_InitStructure);
}

void Car_to_interface (char s) {
	static int counter;
	if (counter) {
		cmd_PC_board[counter++] = ETX;
	}
	if (s != 'Q' ){
		cmd_PC_board[counter++] = s;
	}	else {
		cmd_PC_board[++counter] = ETX;	
		
		uint8_t check_somme = 0;
		uint8_t check_somme_carry = 0;
		uint8_t counter = 1;
		uint8_t i ;
		
		for(i=0 ; i < counter-1 ; i++) {
			if(check_somme+cmd_PC_board[i] > 255) check_somme_carry = 1;
			check_somme += cmd_PC_board[i];
		}
		// Check somme final calcul and display
		cmd_PC_board[counter-1] = ~((check_somme+check_somme_carry+ETX));			
		counter = 0;
		
	}
}


void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE))	// USART_IT_RXNE is clear by callin the USART_ReceiveData function (doc)
  {
		Car_to_interface(USART_receiver(USART3));
		//PC_to_Board(cmd_PC_board, USART3);
  }

}

