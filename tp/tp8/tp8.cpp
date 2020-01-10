#include <avr/io.h>
#include "debug.h"
#include "uart.h"
#include "del.h"
#include "commun.h"

DEL del;
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
        commun::delai_ms(10);
        if (PIND & 0x04){
            return true;
        }
    }

    return false;
}


void doAction(State &currentState){
    switch (currentState) {
            case INIT:
            
                del.allumerDEL(OFF);
                break;

            case A:

                del.allumerDEL(OFF);
                break;

            case B:
            
                del.allumerDEL(OFF);
                break;

            case C:
            
                del.allumerDEL(OFF);
                break;
            
            case D:
           
                del.allumerDEL(OFF);
                break;

            case E:
            
                del.allumerDEL(ROUGE, 1000);
                break;


        }
}
void changeState(State &currentState){
    
    switch (currentState) {
        
            case INIT:
            
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
  
    State currentState = State::INIT;

    

    for(;;){
         DEBUG_PRINT(currentState);
        changeState(currentState);
        doAction(currentState);
        
    }

    return 0;
}
