#include "Handler.h"

extern char commande[cmd_length_max];
extern uint8_t command_sent;

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
	static int counter = 0;
	/*
	if (s != ETX && s != STX) {
		cmd[counter++] = s ;
	} else if (s == ETX) {
		cmd[counter-1] = '\0';
		IRQ_main_connection(cmd, 0);
	}	
	*/
	if (s != 0xD) {
		commande[counter++] = s ;
	} else {
		command_sent = 1;
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

