/*Problème 1: Atténuation d'une DEL


Comme on l'a vu durant la première semaine, une DEL ne peut qu'être éteinte ou allumée.
Par contre, on peut tromper l'œil humain. L'exercice demandé ici consiste à faire atténuer progressivement la luminosité d'une DEL.
Autrement dit, de faire comme si la DEL s'éteignait en 3 secondes plutôt qu'abruptement. Cet objectif est réalisable facilement en
diminuant tranquillement le rapport a/b d'un signal PWM qui alimente une DEL. On devra donc retourner au code écrit pour la première
semaine, mais en générant un signal PWM pour alimenter la DEL. De plus, on devra contrôler encore une fois le moment où la valeur
d'un signal de sortie passe de 0 à 1 et de 1 à 0 et compter les durées passées au niveau haut et au niveau bas. Par contre, la
fréquence (inverse de la valeur de «b») devra demeurer constante à 1 kHz. Faites cette atténuation pour la couleur rouge dans un premier
temps et la couleur verte dans un deuxième temps (ou l'inverse). Comme le microcontrôleur tourne à 8 MHz, il faudra encore lui faire
perdre beaucoup de temps. Bien optimiser votre code pour vous assurer que le microcontrôleur fasse le minimum de calculs dans vos
boucles. Montrez votre code et ce qui se passe sur la carte au chargé de cours pour le contrôle de votre séance.
Reponse: 2000
*/
//Aymeric Labrie 1995467
//Youva Boutora 1986737
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

typedef unsigned char uint8_t;
enum ETAT {INIT, ETAT1 , ETAT2, ETAT3, ETAT4, ETAT5, ETAT6, ETAT7, ETAT8, ETAT9, ETAT10, ETAT11};
const unsigned int FREQUENCE_PWM = 1000; // valeur b du PWM a 1kHz

void allumerDEL(int temps_us){
    PORTD = 0x01;
    for(int i = 0; i < temps_us; i++)
        _delay_us(1);              
    PORTD = 0x00;
}

void pwm(int temps_ms){
    double periode_b_ms = 1000/FREQUENCE_PWM; // Periode = 1/frequence = 1000ms/(s*1000Hz) = 1ms
    double periode_a_ms = periode_b_ms;
    double facteur_diminution_a = periode_b_ms/temps_ms;
    while(periode_a_ms > 0.0){
        allumerDEL(int(periode_a_ms*1000));
        periode_a_ms -= facteur_diminution_a;
        for(int i = 0; i < int((periode_b_ms - periode_a_ms)*1000); i++)
            _delay_us(1);              
    }

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

void changerEtat(ETAT &etat){
    switch (etat){
        case INIT:
            if(button_appuye())
                etat = ETAT1;
            break;
        case ETAT1:
            etat = ETAT2;
            break;
        case ETAT2:
            if(!button_appuye())
                etat = INIT;
            break;
        default:
            etat = INIT;
            break;
    }
}

void effectuerEtat(const ETAT &etat){
    switch (etat){
        case INIT:
            break;
        case ETAT1:
            pwm(3000);
            break;
        case ETAT2:
            break;
        default:
            break;
    }
}

int main() {
    ETAT etat = INIT;
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0xfb; // PORT D1, D3 et D4 en mode sortie. PORT D2 est en mode entree.
    while (true) {
        changerEtat(etat);
        effectuerEtat(etat);
        _delay_ms(100); // faire perdre du temps au microcontrôleur
    }
}
