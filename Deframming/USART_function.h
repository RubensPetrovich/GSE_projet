
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USART3_function
#define USART3_function

#include "commun.h"


// Initialization USART 3
void USART_Initialize(void) ;

// Send String via USART3
void USART_puts(char* s, USART_TypeDef* my_USART);

// Send character via USART3
void USART_putc(uint16_t s, USART_TypeDef* my_USART);

// Get character via USART3
uint16_t USART_receiver(USART_TypeDef* my_USART);

// Send character in binary (8 bits) vuia USART3
void USART_send_binary(uint16_t x, USART_TypeDef* my_USART);

void framming(char*, USART_TypeDef* my_USART);

void PC_to_Board(char*, USART_TypeDef* my_USART);

#endif
