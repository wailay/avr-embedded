/*
 * Andrew Abdo et Wail Ayad 
 * Numero equipe : 12
 * Description: Probleme 1 du TP3 
*/





#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>



int main(){

    // PORT B0 et B1 en sortie;
    DDRA = 0x03;

    //ceci est le nombre pour garder le signal PWM a une frequence de 1KHz, 2000 * 4 = 8000 cycles
    uint16_t b = 2000;
    uint16_t a = 2000;
    uint16_t c = 0;
        for(uint16_t i = 1; i < 3001; i++){
        
       
        PORTA = 0x01;
         if(i % 3 != 0 ){
        _delay_loop_2(a--);
         }
        PORTA = 0x00;
         if(i % 3 != 0 ){
        _delay_loop_2(i);
        
        }
        }
        
    

    return 0;
}
