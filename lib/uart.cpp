/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente la transmission et reception serielle a l'aide de USART
 * 
*/
#include "uart.h"

UART::UART(){
    // permettre la réception et la transmission par le UART0
    UBRR0H = 0;
    UBRR0L = 0xCF;

    //Enable Transmission and reception
    UCSR0B |= (1<<RXEN0) | (1 << TXEN0);

    // Format des trames: 8 bits (UCSZ01 et UCSZ00) (3<<UCSZn0) par defaut deja a 1 
    //, 1 stop bits (USBSn = 0), none parity (UPMN) ;
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);    
}

void UART::transmissionUART(uint8_t donnee){
    //Wait for empty transmit buffer
    while(!(UCSR0A & (1<< UDRE0)));
    UDR0 = donnee;
}

unsigned char UART::receptionUART(){
   //Attente de donnee
   while (!(UCSR0A & (1<<RXC0)) );
   return UDR0;
};