/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 1 du TP6
 * 
 * Entree : Boutton poussoire (B), CT120 (Entree à 1 lorsquil atteint 120)
 * Sortie : OFF, VERT, ROUGE
 * Les etats INIT, A (Etat_Compteur_Start), B (Etat_Clignote_Vert), C(Etat_Fait_Rien), D (Etat_Clignote_Rouge), E (Etat_Vert_1sec)
 * 
 * +----------------+------+-------+---------------+--------+
 * | Etats Presents |   B  | CT120 | Etats Futures | Sortie |
 * +----------------+------+-------+---------------+--------+
 * | INIT           | 0    | 0     | INIT          | OFF    |
 * +----------------+------+-------+---------------+--------+
 * | INIT           | 1    | X     | A             | OFF    |
 * +----------------+------+-------+---------------+--------+
 * | A              | 1    | 0     | A             | OFF    |
 * +----------------+------+-------+---------------+--------+
 * | A              | 0    | X     | B             | OFF    |
 * +----------------+------+-------+---------------+--------+
 * | A              | X    | 1     | B             | OFF    |
 * +----------------+------+-------+---------------+--------+
 * | B              | X    | X     | C             | VERT   |
 * +----------------+------+-------+---------------+--------+
 * | C              | X    | X     | D             | OFF    |
 * +----------------+------+-------+---------------+--------+
 * | D              | X    | X     | E             | ROUGE  |
 * +----------------+------+-------+---------------+--------+
 * | E              | X    | X     | INIT          | VERT   |
 * +----------------+------+-------+---------------+--------+
 * 
 * LED : (+) PORT B1 et (-) PORT B0
 *   
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

enum State
{
    INIT,
    Etat_Compteur_Start,
    Etat_Clignote_Vert,
    Etat_Fait_Rien,
    Etat_Clignote_Rouge,
    Etat_Vert_1sec
};
volatile State currentState = State::INIT;
volatile uint8_t counter = 0;
volatile bool buttonPressed = false;
volatile bool timerStarted = false;

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

void blinkHalfSecond()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        turnGreen();
        _delay_ms(50);
        turnOff();
        _delay_ms(50);
    }
}

//Sallume 2 fois par seconde pendant compteur/2
void blinkRedCounter()
{
    for (uint8_t i = 0; i < counter / 2; i++)
    {
        turnRed();
        _delay_ms(250);
        turnOff();
        _delay_ms(250);
    }
}
void partirMinuterie(uint16_t duration)
{

    //TCNT = Timer Counter
    //Init du counter value du TIMER1
    TCNT1 = 0;

    //OCR1A = Output Compare Register A
    OCR1A = duration;

    //Timer/Counter Control Register
    //Set Normal port operation OCnA et OCnB disconnected bit 0 0
    TCCR1A = 0;

    //On ajuste le prescaler a 256, et on met le mode CTC
    TCCR1B |= _BV(CS12) | _BV(WGM12);

    TCCR1C = 0;

    //Enable le Timer Interrupt du compare avec A
    TIMSK1 |= _BV(OCIE1A);
}


void changeState()
{
    switch (currentState)
    {

    case INIT:

        if (buttonPressed)
        {
            currentState = Etat_Compteur_Start;
        }

        break;

    case Etat_Compteur_Start:

        if (!buttonPressed || counter == 120)
        {
            currentState = Etat_Clignote_Vert;
        }

        break;
    //Les délais des états se trouvent dans doAction()
    case Etat_Clignote_Vert:

        currentState = Etat_Fait_Rien;
        break;
    case Etat_Fait_Rien:

        currentState = Etat_Clignote_Rouge;
        break;
    case Etat_Clignote_Rouge:

        currentState = Etat_Vert_1sec;
        break;
    case Etat_Vert_1sec:

        currentState = INIT;
        break;
    }
}

void doAction()
{
    switch (currentState)
    {
    case INIT:
        turnOff();
        //Initialisation des conditions
        counter = 0;
        timerStarted = false;
        break;

    case Etat_Compteur_Start:
        turnOff();

        //Partir la minuterie avec un compte de 3124, calcul avec un prescaler de 256
        //Si la minuterie est deja partie, ne pas la repartir
        if (!timerStarted)
        {
            partirMinuterie(3124);
            timerStarted = true;
        }

        break;
    case Etat_Clignote_Vert:

        //Arreter la minuterie
        TIMSK1 &= ~(_BV(OCIE1A));

        blinkHalfSecond();

        break;
    case Etat_Fait_Rien:

        turnOff();
        _delay_ms(2000);
        break;
    case Etat_Clignote_Rouge:

        blinkRedCounter();
        break;
    case Etat_Vert_1sec:

        turnGreen();
        _delay_ms(1000);
        break;
    }
}
void init()
{
    cli();

    DDRB |= _BV(PB0) | _BV(PB1);
    DDRD &= ~(_BV(PD2));
    //permettre les interruptions externes
    EIMSK |= _BV(INT0);

    EICRA = _BV(ISC00); //falling edge

    sei();
}

//routine dinterruption en comparaison avec OCR1A
ISR(INT0_vect)
{
    _delay_ms(30);

    //Passer l'etat du boutton poussoir (PIND) a une bool volatile;
    buttonPressed = !(PIND & 0x04);

    EIFR |= _BV(INTF0);
}

ISR(TIMER1_COMPA_vect)
{

    timerStarted = true;
    counter++;
}
int main()
{

    init();

    for (;;)
    {

        changeState();
        doAction();
    }
}
