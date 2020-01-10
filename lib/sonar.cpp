/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente le sonar pour detecter des objets a proximites.
 * 
*/

#include "sonar.h"

Sonar::Sonar()
{
    //Mettre le A0 en mode sortie et le pin A1 en mode entree
    DDRA |= (1 << PA0);
}
/*
 * Pour commencer a sonder un objet il faut envoyer un signal de trigger, pendant environ 10us minimum. 
 * La pin pour le signal trigger est A0 
*/

// Attends jusqu'à recevoir un signal
void Sonar::ecouterEcho()
{
    while (!(PINA & _BV(PA1)))
    {
    }
    int count = 0;
    while ((PINA & _BV(PA1)))
    {
        _delay_us(0.5);
        count++;
    };
    wavelength = count;
    distance = wavelength / 58;
}

void Sonar::envoyerSignal()
{
    cli();
    PORTA |= _BV(PA0);
    _delay_us(15);
    PORTA &= ~(1 << PA0);
    sei();
}
