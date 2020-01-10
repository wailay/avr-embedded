//Aymeric Labrie 1995467

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "memoire_24.h"


void initialisationUART ( void ) {
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premier registres pour vous éviter des complications
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A = 0;
    UCSR0B = (1 << TXEN0) | (1 << );
    // Format des trames: 8 bits, 1 stop bits, none parity
    UCSR0C = (1 << UPM01) | (1 << UPM00) | (1 << USBS0) | (1 << UCSZ11) | (1 << UCSZ10);
}



// De l'USART vers le PC
void transmissionUART ( uint8_t donnee ) {
    
}

int main(){
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xff; // PORT D est en mode sortie

    char mots[21] = "Le robot en INF1900\n";
    uint8_t i, j;
    for ( i = 0; i < 100; i++ ) {
        for ( j=0; j < 20; j++ ) {
            transmissionUART ( mots[j] );
        }
    }    
    
}