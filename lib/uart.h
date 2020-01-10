/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente la transmission et reception serielle a l'aide de USART
 * 
*/

#ifndef UART_H
#define UART_H

#include "headers.h"

class UART
{
public:
    UART();
    void transmissionUART(uint8_t donnee);
    unsigned char receptionUART();
};
#endif