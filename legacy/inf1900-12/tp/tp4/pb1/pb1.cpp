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

// Initialisation des etats
enum State{
    INIT,
    A,
    B,
    C,
    D,
    E,
};

volatile State etat = State::INIT;
volatile bool bouttonAppuye = false;

void init(){
    cli();

    DDRA |= (1 << PA0) | (1 << PA1);

    //permettre les interruptions externes
    EIMSK |= (1 << INT0) ;

    EICRA |= (1 << ISC00); //any edge
    
    sei();
}


bool debounce(){
    if (PIND & 0x04){
        _delay_ms(10);
        if (PIND & 0x04){
            return true;
        }
    }
    return false;
}
void turnRed(){
    //Decaler de 1 vers la gauche le bit 0000 0001 => 0000 0010
    PORTA &= ~(1 << 0);
    PORTA |= (1 << 1);
}
void turnGreen(){
    PORTA &= ~(1 << 1);
    PORTA |= (1 << 0);
}
void turnOff(){
    //clear les bits 0 et 1
    PORTA &= ~(1 << 0);
    PORTA &= ~(1 << 1);
} 
//Fonction qui tourne la led ambre
void turnAmbre(){
    while (debounce())
    {
        
        PORTA |= (1 << 1);
        PORTA &= ~(1 << 0);
        _delay_ms(8);
        PORTA |= (1 << 0);
        PORTA &= ~(1 << 1);
        _delay_ms(8);
    };
}

void doAction(){
    switch (etat) {
            case INIT:
            
                turnRed();
                break;

            case A:

                turnAmbre();
                break;

            case B:
            
                turnGreen();
                break;

            case C:
            
                turnRed();
                break;
            
            case D:
           
                turnOff();
                break;

            case E:
            
                turnGreen();
                break;


        }
}
void changeState(){
    switch (etat) {
            case INIT:
                etat = A;
                break;

            case A:

                etat = B;
                break;

            case B:
            
                etat = C;
                break;

            case C:
            
                etat = D;
                break;
            
            case D:
            
                etat = E;
                break;

            case E:
            
                etat = INIT;
                break;


        }
}


ISR(INT0_vect){
    _delay_ms(30);

    if (PIND & 0x04){ }
    changeState();
    EIFR |= (1 << INTF0) ;
    }


int main(){

    
   
    init();

    for(;;){

        

        doAction();

    }

    return 0;
}

