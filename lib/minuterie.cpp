/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente la transmission et reception serielle a l'aide de USART
*/

#include "minuterie.h"

void startTimer0CTC(uint8_t duration, const uint8_t PBx)
{
    //OC0A
    //Ce timer sera utilise pour le piezo electrique, de ce fait on permet le choix a une pin adjacente
    //detre mis en mode sortie, car le piezo a besoin d'un 5V (timer) et dun ground.
    DDRB |= (1 << PB3) | (1 << PBx);
    TCNT0 = 0;
    OCR0A = duration;
    TCCR0A |= (1 << COM0A0) | (1 << WGM01); // toggle OC0A on compare PB3;
    TCCR0B |= (1 << CS02);                  //prescaler 256, for midi notes
}

void stopTimer0CTC()
{
    TCCR0A = 0;
    TCCR0B = 0;
    OCR0A = 0;
}

void partirMinuterie2CTC()
{
    TCCR2A = 0; //normal mode, le compteur TCNT2 va compter jusqua 255 et ensuite retombe 0
    //prescaler de 1024, pour atteindre le delai de 30ms du sonar sans overflow
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    TCNT2 = 0;
}

void arreterMinuterie2CTC()
{
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
}