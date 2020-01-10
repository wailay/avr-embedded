//Aymeric Labrie 1995467

#define F_CPU 8000000
//#define F_TIMER1 32768
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
enum ETAT : uint8_t {INIT, ETAT1, ETAT2};
enum COULER : uint8_t {FERMEE, VERT, ROUGE};
volatile uint8_t minuterieExpiree; 
volatile uint8_t boutonPoussoir;

void initialisation(){
    cli();
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xfb; // PORT D1, D3 et D4 en mode sortie. PORT D2 est en mode entree.
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00);
    sei();
}

void partirMinuterie(uint16_t duree) {
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    TCCR1A |= (1 << COM1A1); //Clear OCnA/OCnB on Compare Match (Set output tolow level).
    TCCR1B |= (1 << CS02);
    TCCR1B |= (1 << CS00);
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A); //Timer/Counter1, Output Compare A Match Interrupt Enable
    TCNT1 = 0;
    TCCR1C = 0;
    OCR1A = duree;
}

void allumerDEL(COULER couleur){
    switch(couleur) {
    case FERMEE:
        PORTA = 0x00;
        break;
    case VERT:
        PORTA = 0x02;
        break;
    case ROUGE:
        PORTA = 0x01;
        break;    
    default:
        break;
    }
}



void changerEtat(ETAT etat){
    switch(etat){
        case INIT:
            etat = ETAT1;
            break;
        case ETAT1:
            break;
        default:
            //etat = INIT;
            break;
    }
}

void effectuerEtat(ETAT etat){
    switch(etat){
        case INIT:
            for(uint8_t i = 0; i < 3; i++)
                _delay_ms(1000);
            allumerDEL(VERT);
            _delay_ms(100);
            partirMinuterie(F_CPU/1024); // 1 seconde
            allumerDEL(FERMEE);
            minuterieExpiree = 0;
            boutonPoussoir = 0;
            do{
            } while (!(boutonPoussoir || minuterieExpiree));
            if(boutonPoussoir)
                allumerDEL(VERT);
            else
                allumerDEL(ROUGE);
        case ETAT1:
            break;
        default:
            break;
    }
}

ISR(TIMER1_COMPA_vect){
    minuterieExpiree = 1;
}

ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04)
        boutonPoussoir = 1;
}

int main(){
    ETAT etat = INIT;
    initialisation();
    effectuerEtat(etat);
    changerEtat(etat);
    while(true){
        _delay_ms(100); // faire perdre du temps au microcontrôleur
    }
    
}