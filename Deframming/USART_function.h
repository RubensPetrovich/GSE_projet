
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USART3_function
#define USART3_function

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32F4xx.h"
#include "STM32F4xx.h"


#define cmd_length_max 64	// ATTENTION if more than 64, use for the comter uint16_t (or int) insted of uint8_t !!
#define STX 0x02
#define ETX 0x03

extern char cmd_PC_board [cmd_length_max];

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
