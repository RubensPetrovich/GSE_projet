
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef my_HANDLER_H
#define my_HANDLER_H

#include "commun.h"
#include "Handler.h"
#include "USART_function.h"

void NVIC_Configuration(void);

void USART3_IRQHandler(void);


#endif
