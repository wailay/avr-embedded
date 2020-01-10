/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Fichier principal regroupant la classe Robot et le fonctionnement deux boutons.
 * 
 * Voir le fichier LISEZMOI.txt pour l'identification materielles
 * 
*/
#include "debug.h"
#include "robot.h"
#include "interrupt.h"
#include "minuterie.h"
#include "robot.h"
#include "del.h"
#include "debug.h"
#include "display.h"
#include "sonar.h"
#include "uart.h"

//Initialisation des bouttons
Interrupt bouttonBlanc(1, FALLING_EDGE);
Interrupt bouttonPoussoir(0, RISING_EDGE);

volatile uint8_t nbAppuies = 0;
volatile bool boutonInterruptAppuye = false;
volatile bool actualiserLCD = true; //Cette boolean est utile pour ne pas ecrire dans le display LCD a linfini, mais seulement une fois.

//interrupt causé par le bouton blanc
ISR(INT1_vect)
{
    //anti rebond
    _delay_ms(30);
    //negation car le boutton blanc agit de facon inverse
    if (!bouttonBlanc.isButtonPressed())
    {
        _delay_ms(10);
        if (!bouttonBlanc.isButtonPressed())
        {
            nbAppuies++;
            actualiserLCD = true;
        }
    }
    EIFR |= (1 << INTF1);
}

//Interrupt cause par le boutton poussoir
ISR(INT0_vect)
{
    //anti rebond
    _delay_ms(30);
    if (bouttonPoussoir.isButtonPressed())
    {
        _delay_ms(10);
        if (bouttonPoussoir.isButtonPressed())
        {
            boutonInterruptAppuye = true;
        }
    }
    EIFR |= (1 << INTF0);
}

int main()
{

    /********************************
    *   DEBUT PROGRAMME PRINCIPAL   *
    *********************************/
    Robot robot;

    //Detecter nb appuie bouton blanc jusqu'à l'appuie du bouton interrupt
    while (!boutonInterruptAppuye)
    {
        //Changer l'etat seulement lorsque le boutton est appuye, ce qui fait que le display LCD ne se rafraichit qu'une seule fois.
        if (actualiserLCD)
        {
            switch (nbAppuies % 4) // Selection circulaire de la section avec le bouton blanc
            {
            case COULOIR:
                robot.setSectionInitial(COULOIR);
                break;
            case MUR:
                robot.setSectionInitial(MUR);
                break;
            case BOUCLES:
                robot.setSectionInitial(BOUCLES);
                break;
            case COUPURES:
                robot.setSectionInitial(COUPURES);
                break;
            }
            robot.afficherSection();
            actualiserLCD = false;
        }
    }

    robot.effectuerRoutineDebut(); //routine de 2 secondes exactement
    while (!robot.estTerminee())
    {
        robot.effectuerEtat();
        robot.changerEtat();
        robot.afficherSection();
    }
}