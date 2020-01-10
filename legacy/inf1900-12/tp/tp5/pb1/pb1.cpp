/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 1 du TP4 
 * 
*/


#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "memoire_24.h"

void init(){
    cli();

    DDRA |= (1 << PA0) | (1 << PA1);

    //permettre les interruptions externes
    EIMSK |= (1 << INT0) ;

    EICRA |= (1 << ISC00); //any edge
    
    sei();
}

int main(){
    Memoire24CXXX memoire;
    char data[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    char rx[sizeof(data)];
    
    memoire.ecriture(0x00, (uint8_t *) data, sizeof(data));
    _delay_ms(5);
    //ecrire 0x00 a la fin de la chaine.
    memoire.ecriture(0x00 + sizeof(data), 0xFF);
    _delay_ms(5);
    memoire.lecture(0x00, (uint8_t *) rx, sizeof(data) + 1);

    DDRA = 0xFF;
    bool same = false;
    // receive_data = 0xFF;
    for(int i =0; i < sizeof(data); i++){
        if(data[i] == rx[i]) same = true;
    }
    
    if(same){
        PORTA = 0x01;
    }else{
        PORTA = 0x02;
    }
    return 0;
}

