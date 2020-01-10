

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "memoire_24.h"


void init(){
    
    DDRD |= (1 << PD0) | (1 << PD1);
}
void initUART(){
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premier registres pour vous éviter des complications
    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0
    // UCSR0A = '';
    // il nest pas necessaire dajuster ce registre 

    //Enable Transmission and reception
    UCSR0B |= (1<<RXEN0) | (1 << TXEN0);

    // Format des trames: 8 bits (UCSZ01 et UCSZ00) (3<<UCSZn0) par defaut deja a 1 
    //, 1 stop bits (USBSn = 0), none parity (UPMN) ;
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
}

void transmissionUART(uint8_t donnee){
    //Wait for empty transmit buffer
    while(!(UCSR0A & (1<< UDRE0)))
                ; //**** IMPORTANT de mettre le point virgule
    UDR0 = donnee;
}

int main(){
    Memoire24CXXX memoire;
    init();
    initUART();
    uint8_t rxData;
    
    // memoire.lecture(0x00, &rxData,1);
    // transmissionUART(rxData);
    // memoire.lecture(0x00 + 1, &rxData,1);
    // transmissionUART(rxData);
    uint16_t i = 0;
    do{
    memoire.lecture(0x00 + i, &rxData);
    transmissionUART(rxData);
    i++;
    }while(rxData != 0xFF);
    

    
    return 0;
}