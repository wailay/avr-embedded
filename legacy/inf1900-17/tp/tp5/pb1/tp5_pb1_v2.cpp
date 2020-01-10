//Aymeric Labrie 1995467

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "memoire_24.h"
void testerDEL(){
    PORTA = 0x01;
    _delay_ms(500);
    PORTA = 0x02;
    _delay_ms(500);
    PORTA = 0x01;
    _delay_ms(500);
    PORTA = 0x00;
    _delay_ms(500);
}

int main(){
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xff; // PORT D est en mode sortie
    testerDEL();
    static const char poly[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    static const uint16_t adresse1 = 0x0000;
    static const uint16_t adresse2 = sizeof(poly);
    Memoire24CXXX mem;
    mem.ecriture(adresse1, (uint8_t*)poly, sizeof(poly));
    mem.ecriture(adresse2, uint8_t(0x00));
    bool aucuneErreur = true;
    uint8_t octet = 0x00;
    for(uint16_t i = 0; i < sizeof(poly); i++){
        _delay_ms(5);
        mem.lecture(i, &octet);
        if(uint8_t(poly[i]) != octet)
            aucuneErreur = false;
    }
    if(aucuneErreur)
        PORTA = 0x01;
    else
        PORTA = 0x02;
    while (true)
    {
    }
    
}