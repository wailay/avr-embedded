/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 1 du TP2 
 * Diagramme :
 * 
 * Entree Int : 1 = nouveau click
 *                 | Entree|                |  Sortie  |
 * | Etats Present |  Int  |  Etats futurs  |    DEL   |
 * | ------------- | ----- | -------------  | -------- |
 * |    Init       |   0   |      INIT      |     0    |    
 * |    Init       |   1   |       A        |     0    |
 * |    A          |   0   |       A        |     0    |
 * |    A          |   1   |       B        |     0    |
 * |    B          |   0   |       B        |     0    |
 * |    B          |   1   |       C        |     0    |
 * |    C          |   0   |       C        |     0    |
 * |    C          |   1   |       D        |     0    |
 * |    D          |   0   |       D        |     0    |
 * |    D          |   1   |       E        |     0    |
 * |    E          |   0   |      INIT      |     1    |
 * |    E          |   1   |      INIT      |     1    |
 * 
 * BROCHE B2 au + LED
 * BORCHE B1 au - LED
 * 
*/




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
    //Decaler de 1 vers la gauche le bit 0000 0001 => 0000 0010 et l'assigner avec un OR et CLEAR le 0eme bit avec AND
    PORTB &= ~(1 << 0);
    PORTB |= (1 << 1);
}


void turnOff(){
    //clear les bits 0 et 1
    PORTB &= ~(1 << 0);
    PORTB &= ~(1 << 1);
} 

void doAction(State &currentState){
    switch (currentState) {
            case INIT:
            
                turnOff();
                break;

            case A:

                turnOff();
                break;

            case B:
            
                turnOff();
                break;

            case C:
            
                turnOff();
                break;
            
            case D:
           
                turnOff();
                break;

            case E:
            
                turnRed();
                _delay_ms(1000);
                break;


        }
}
void changeState(State &currentState){
    switch (currentState) {
            case INIT:
                //Ecouter le deboune de l'interrupteur et ensuite lock le programme avec le boucle while
                //Des que l'interrupteur est relacher, changer d'etat.
                if (debounce()){
                    while (debounce()){
                    }
                    currentState = A;
                }
                break;

            case A:

                if (debounce()){
                    while (debounce()){
                    }
                    currentState = B;
                }
                break;

            case B:
            
                if (debounce()){
                    while (debounce()){
                    }
                    currentState = C;
                }
                break;

            case C:
            
                if (debounce()){
                    while (debounce()){
                    }
                    currentState = D;
                }
                break;
            
            case D:
            
                if (debounce()){
                    while (debounce()){
                    }
                    currentState = E;
                }
                break;

            case E:
            
                currentState = INIT;
                break;


        }
}
int main(){

    //Enable pin interrupteur en entree
    DDRD = 0x00;
    //Mettre le port B0 et B1 de la led en sortie
    DDRB = 0x03;
   
    State currentState = State::INIT;

   

    for(;;){
        doAction(currentState);
        changeState(currentState);
    }

    return 0;
}
