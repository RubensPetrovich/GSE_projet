
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef my_HANDLER_H
#define my_HANDLER_H

#include "Handler.h"
#include "USART_function.h"

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32F4xx.h"
#include "STM32F4xx.h"

extern char cmd_PC_board [cmd_length_max];

void NVIC_Configuration(void);

void USART3_IRQHandler(void);


#endif
