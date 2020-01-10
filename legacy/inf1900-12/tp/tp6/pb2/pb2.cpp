/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 2 du TP6 
 * Nous avons mesuré avec le multimetre la valeur de tension de reference maximale lorsque la 
 * photoresistance est sous une lumiere forte d'une lampe. Cette tension est de 
 * 4.91V. Ensuite, nous avons mesuré la tension de reference minimum lorsque la photoresistance est caché,
 * nous avons trouvé 2.3V
 * Lumiere Forte = 4.91V
 * Lumiere Faible = 2.3V
 * 
 * Seuil_MAX ~= 4.91/5 * 255 = 250
 * Seuil MIN ~= 2.3V/5 * 255 = 117
 * 
 * On divise par 255, car le CAN nous redonne 8bits de data sur 10.
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "can.h"

const uint8_t REF_VOLT = 5;
const uint8_t MAX = 4.9;
const uint8_t AMBIANT = 3.605;
const uint8_t MIN = 2.3;

void turnRed()
{
    PORTB &= ~(_BV(PB0));
    PORTB |= _BV(PB1);
}

void turnGreen()
{
    PORTB &= ~(_BV(PB1));
    PORTB |= _BV(PB0);
}

void turnOff()
{
    PORTB &= ~(_BV(PB1));
    PORTB &= ~(_BV(PB0));
}

void turnYellow()
{
    turnGreen();
    _delay_ms(20);
    turnRed();
    _delay_ms(5);
}

void init()
{
    //Mettre le port A0 en entree pour lire les data analogique du capteur de lumiere
    DDRA &= ~(1 << PA0);
    DDRB |= (1 << PB0) | (1 << PB1);
}

//0xFF, car la conversion analogique a numerique retourne sur 8 bits sur 10 de données, donc 255 cest le max
const uint8_t SEUIL_MAX = (4.91 / 5) * 0xFF;
const uint8_t SEUIL_MIN = (2.3 / 5) * 0xFF;

int main()
{
    init();

    //Initialisation du can
    can conv;

    for (;;)
    {

        //On veux ADC0 donc MUX4:0 doit etre 0000, dou PA0 = 0 selon AVRlibc
        uint8_t lectureCan = (conv.lecture(PA0) >> 2);

        if (lectureCan >= SEUIL_MAX)
        {
            turnRed();
        }
        else if (lectureCan > SEUIL_MIN && lectureCan < SEUIL_MAX)
        {
            turnYellow();
        }
        else
        {
            turnGreen();
        }
    }
}