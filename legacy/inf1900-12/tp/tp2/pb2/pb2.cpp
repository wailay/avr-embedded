/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 2 du TP2 
 * Diagramme :
 * 
 * Sortie : DEL : V = Verte, R = Rouge, Off = Eteinte, Am = Ambre
 *                 | Entree|                |  Sortie  |
 * | Etats Present |  Int  |  Etats futurs  |    DEL   |
 * | ------------- | ----- | -------------  | -------- |
 * |    Init       |   0   |      Init      |     R    |    
 * |    Init       |   1   |       A        |     R    |
 * |    A          |   0   |       B        |     Am   |
 * |    A          |   1   |       A        |     Am   |
 * |    B          |   0   |       B        |     V    |
 * |    B          |   1   |       C        |     R    |
 * |    C          |   0   |       D        |    Off   |
 * |    C          |   1   |       C        |     R    |
 * |    D          |   0   |       D        |    Off   |
 * |    D          |   1   |       E        |     V    |
 * |    E          |   0   |      Init      |     R    |
 * |    E          |   1   |       E        |     V    |
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
    //Decaler de 1 vers la gauche le bit 0000 0001 => 0000 0010
    PORTB &= ~(1 << 0);
    PORTB |= (1 << 1);
}

void turnGreen(){
    PORTB &= ~(1 << 1);
    PORTB |= (1 << 0);
}

void turnOff(){
    //clear les bits 0 et 1
    PORTB &= ~(1 << 0);
    PORTB &= ~(1 << 1);
} 
//Fonction qui tourne la led ambre
void turnAmbre(){
    while (debounce())
    {
        
        PORTB |= (1 << 1);
        PORTB &= ~(1 << 0);

        _delay_ms(10);

        PORTB |= (1 << 0);
        PORTB &= ~(1 << 1);
        _delay_ms(5);   
    };
}


void doAction(State &currentState){
    switch (currentState) {
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
void changeState(State &currentState){
    switch (currentState) {
            case INIT:

                if (debounce()) currentState = A;
                break;

            case A:

                if (!debounce()) currentState = B;
                break;

            case B:
            
                if (debounce()) currentState = C;
                break;

            case C:
            
                if (!debounce()) currentState = D;
                break;
            
            case D:
            
                if (debounce()) currentState = E;
                break;

            case E:
            
                if (!debounce()) currentState = INIT;
                break;


        }
}
int main(){

    //Enable pin interrupteur en entree
    DDRD = 0x00;

    //Mettre le port B0 et B1 en sortie
    DDRB = 0xff;
   
    State currentState = State::INIT;

   

    for(;;){
        doAction(currentState);
        changeState(currentState);
    }

    return 0;
}
