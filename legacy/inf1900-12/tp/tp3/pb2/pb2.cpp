/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 2 du TP3 
*/





#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>



void tournerA60Hz(uint16_t a, uint16_t b){
         for(uint16_t i = 0; i < 120; i++){
        PORTA = 0x01;
        _delay_loop_2(a);
        PORTA = 0x00;
        _delay_loop_2(b-a);
        }
}
void tournerA400Hz(uint16_t a, uint16_t b){
         for(uint16_t i = 0; i < 800; i++){
        PORTA = 0x01;
        _delay_loop_2(a);
        PORTA = 0x00;
        _delay_loop_2(b-a);
        }
}


int main(){
    
    // PORT B0 et B1 en sortie;
    DDRA = 0x03;
    

    // Pour une periode de 60Hz il faut 128000 cycles
    uint16_t b = 33333;
    uint16_t a;
    //25 % 
    a = b/4;
    tournerA60Hz(a, b);
    

    //50 %
    a = b/2;
    tournerA60Hz(a, b);

    //75 %
    a = b * (0.75);
    tournerA60Hz(a, b);

        
    //100%
    tournerA60Hz(b, b+1);

    b = 5000;
    tournerA400Hz(b/4, b);
    tournerA400Hz(b/2,b);
    tournerA400Hz(b * (0.75), b);
    tournerA400Hz(b, b+1);
    return 0;
}
