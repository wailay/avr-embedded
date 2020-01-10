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
// ETAT6         1  INIT


#define F_CPU 8000000
#include <avr/io.h> 
#include <util/delay.h>

typedef unsigned char uint8_t;                                                                                                                                                             

enum ETAT {INIT, ETAT2, ETAT3, ETAT4, ETAT5, ETAT6};
enum COULEUR {FERMEE, ROUGE, VERT, AMBRE};

void modifierDEL(COULEUR couleur){
    switch (couleur) {
        case FERMEE:
            PORTD = 0x00;
            break;
        case ROUGE:
            PORTD = 0x01;
            break;
        case VERT:
            PORTD = 0x02;
            break;
        case AMBRE:
            for(int i = 0; i < 20; i++){
                PORTD = 0x01;
                _delay_ms(2);
                PORTD = 0x02;
                _delay_ms(2);
            }
            break;
        default:
            break;
    }
}

bool button_appuye(){
    if(PIND & 0x04){
        _delay_ms(10);
        if(PIND & 0x04)
            return true;
        else
            return false;
    }
    return false;
}

int main() {
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xfb; // PORT D1, D3 et D4 en mode sortie. PORT D2 est en mode entree.
    uint8_t etat = INIT;
    while(true){
        switch (etat){
            case INIT:
                modifierDEL(ROUGE);
                if(button_appuye())
                    etat = ETAT2;
                else
                    etat = INIT;
                break;
            case ETAT2:
                while(button_appuye()){
                    modifierDEL(AMBRE);
                }
                etat = ETAT3;                               
                break;
            case ETAT3:
                modifierDEL(VERT);
                if(button_appuye())
                    etat = ETAT4;
                else
                    etat = ETAT3;
                break;                
            case ETAT4:
                modifierDEL(ROUGE);
                if(button_appuye())
                    etat = ETAT4;
                else
                    etat = ETAT5;                               
                break;
            case ETAT5:
                modifierDEL(FERMEE);
                if(button_appuye())
                    etat = ETAT6;
                else
                    etat = ETAT5;
                break;
            case ETAT6:
                modifierDEL(VERT);
                if(button_appuye())
                    etat = ETAT6;
                else
                    etat = INIT;
                break;
            default:
                etat = INIT;
                break;
        }
    }
}