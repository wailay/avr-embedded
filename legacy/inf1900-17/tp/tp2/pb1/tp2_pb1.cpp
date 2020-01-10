//Aymeric Labrie 1995467
//Youva Boutora 1986737
// ETAT PRESENT  A  ETAT SUIVANT
// INIT          0  INIT
// INIT          1  ETAT1
// ETAT1         0  ETAT1
// ETAT1         1  ETAT2
// ETAT2         0  ETAT2
// ETAT2         1  ETAT3
// ETAT3         0  ETAT3
// ETAT3         1  ETAT4
// ETAT4         0  ETAT4
// ETAT4         1  ETAT5
// ETAT5         0  ETAT5
// ETAT5         1  ETAT6
// ETAT6         0  ETAT6
// ETAT6         1  ETAT7
// ETAT7         0  ETAT7
// ETAT7         1  ETAT8
// ETAT8         0  ETAT8
// ETAT8         1  ETAT9
// ETAT9         0  ETAT9
// ETAT9         1  ETAT10
// ETAT10        0  ETAT10
// ETAT10        1  ETAT11
// ETAT11        0  ETAT11
// ETAT11        1  INIT



#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay.h>

typedef unsigned char uint8_t;
enum ETAT {INIT,ETAT1 ,ETAT2, ETAT3, ETAT4, ETAT5, ETAT6, ETAT7, ETAT8, ETAT9, ETAT10, ETAT11};

void allumerDEL(){
    PORTD = 0x01;
    _delay_ms(1000);
    PORTD = 0x00;
}

bool button_appuye(){
    if(PIND&0x04){
        _delay_ms(10);
        if(PIND & 0x04)
            return true;
        else
            return false;
    }
    return false;
}

int main() {
    uint8_t etat = INIT;
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xfb; // PORT D1, D3 et D4 en mode sortie. PORT D2 est en mode entree.

    while(true){
        switch (etat){
            case INIT:
                PORTD = 0x00;
                if(button_appuye())
                    etat = ETAT1;// 1er appuie
                break;
            case ETAT1:
                if(!button_appuye())
                    etat = ETAT2;
                break;
            case ETAT2:
                if(button_appuye())
                    etat = ETAT3;// 2e appuie
                break;
            case ETAT3:
                if(!button_appuye())
                    etat = ETAT4;
                break;
            case ETAT4:
                if(button_appuye())
                    etat = ETAT5;// 3e appuie
                break;
            case ETAT5:
                if(!button_appuye())
                    etat = ETAT6;
                break;
            case ETAT6:
                if(button_appuye())
                    etat = ETAT7;// 4e appuie
                break;
            case ETAT7:
                if(!button_appuye())
                    etat = ETAT8;
                break;
            case ETAT8:
                if(button_appuye())
                    etat = ETAT9;// 5e appuie
                break;
            case ETAT9:
                if(!button_appuye())
                    etat = ETAT10;
                break;              
            case ETAT10:
                allumerDEL();
                etat = ETAT11;
                break;
            case ETAT11:
                if(!button_appuye())
                    etat = INIT;
                break;
            default:
                etat = INIT;
                break;
        }
    }
}