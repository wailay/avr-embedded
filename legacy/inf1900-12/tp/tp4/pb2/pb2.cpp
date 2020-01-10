/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 2 du TP4 
 * 
*/

#define F_CPU 8000000
#include <util/delay.h>

#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;

void turnRed(){
    PORTA |= (1 << PA0);
}
void turnOff(){
    PORTA &= ~(1 << PA0) & ~(1 << PA1);
    
}

void turnGreen(){
    PORTA |= (1 << PA1);
}
void init(){
    cli();
    
    DDRA |= (1 << PA0) | (1 << PA1);

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC00) | (1 << ISC01); //rising edge

    sei();
}
void partirMinuterie ( uint16_t duree ) {

minuterieExpiree = 0;
// mode CTC du timer 1 avec horloge divisée par 1024
// interruption après la durée spécifiée

//TCNT = Timer Counter
//Init du counter value du TIMER1
TCNT1 = 0;

//OCR1A = Output Compare Register A
// Prescaler de 1024 ce qui fait un CTC de 1sec / (1 / (8000000/1024)) + 1 = 7811 = 0x1E83 = duree (si on veux 1sec)
OCR1A = duree;

//Timer/Counter Control Register 
//Set Normal port operation OCnA et OCnB disconnected bit 0 0
TCCR1A = 0;

//Ajuster le prescaler a 1024, pour rendre le F_CPU a 8000000/1024
//Ajuster le mode du TIMER 1 a CTC avec le register ORC1A en TOP 
TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);

TCCR1C = 0;

//Enable le Timer Interrupt du compare avec A 
TIMSK1 |= (1 << OCIE1A) ;

}
//Interrupt du bouton poussoir
ISR(INT0_vect){
    _delay_ms(30);
    boutonPoussoir = 1;
    if (PIND & 0x04){}

}

//Interrupt du timer A
ISR(TIMER1_COMPA_vect){
    minuterieExpiree = 1;

}

int main(){
    init();
    
    for(;;){
    _delay_ms(1000);
    turnRed();
    _delay_ms(100);
    turnOff();
    
    uint16_t duree = 7812;
    partirMinuterie(duree);
    do{
        // Ne rien faire tant que le bouton na pas encore ete pese et la minuterie encore entrain de compter
        // en gros attendre la reponse de lusager
    }while (minuterieExpiree == 0 && boutonPoussoir == 0);
    
    cli();
    
    if (boutonPoussoir == 1 && minuterieExpiree == 0){
        for(;;) {
            turnGreen();
        }
    }
    if(minuterieExpiree == 1 ){
        for(;;){
            turnRed();
        }
    }
    

    }
    return 0;
    
}
