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
#define MASK_ROUE_G 0x03
#define MASK_ROUE_D 0x0C
#define ROUE_G_ON 0xFF
#define ROUE_G_OFF 0x00
#define ROUE_D_ON 0xFF
#define ROUE_D_OFF 0x00
#include <avr/io.h>
#include <util/delay.h>

typedef unsigned char uint8_t;
enum ETAT {INIT, ETAT1, ETAT2, ETAT3, ETAT4, ETAT5, ETAT6, ETAT7, ETAT8, ETAT9, ETAT10, ETAT11};
const unsigned int FREQUENCE_PWM_MOTEUR_1 = 60; // 60Hz
const unsigned int FREQUENCE_PWM_MOTEUR_2 = 60; // 400Hz
const unsigned int TEMPS_ROULER_MS = 2000;

void avancerRobot(const unsigned int pwm, const unsigned int frequence_pwm, const unsigned int temps_ms){
    double periode_b_ms = 1000/frequence_pwm; // Periode = 1/frequence = 1000ms/(s*1000Hz) = 1ms
    double periode_a_ms = periode_b_ms*pwm/100;
    double nombreCycles = frequence_pwm*temps_ms/1000;
    for (int i = 0; i < nombreCycles; i++) {
        PORTD =  (PORTD & MASK_ROUE_G) + ROUE_G_ON;
        PORTD =  (PORTD & MASK_ROUE_D) + ROUE_D_ON;
        for(int k = 0; k < int(periode_a_ms*1000); k++)
            _delay_us(1);
        PORTD =  (PORTD & MASK_ROUE_G) + ROUE_G_OFF;
        PORTD =  (PORTD & MASK_ROUE_D) + ROUE_D_OFF;
        for(int k = 0; k < int((periode_b_ms - periode_a_ms)*1000); k++)
            _delay_us(1);   
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

void changerEtat(ETAT &etat){
    switch (etat){
        case INIT:
            if(button_appuye())
                etat = ETAT1;
            break;
        case ETAT1:
            etat = INIT;
            break;
        default:
            etat = INIT;
            break;
    }
}

void effectuerEtat(const ETAT etat){
    switch (etat){
        case INIT:
            break;
        case ETAT1:
            avancerRobot(0, FREQUENCE_PWM_MOTEUR_1, TEMPS_ROULER_MS);
            avancerRobot(25, FREQUENCE_PWM_MOTEUR_1, TEMPS_ROULER_MS);
            avancerRobot(50, FREQUENCE_PWM_MOTEUR_1, TEMPS_ROULER_MS);
            avancerRobot(75, FREQUENCE_PWM_MOTEUR_1, TEMPS_ROULER_MS);
            avancerRobot(100, FREQUENCE_PWM_MOTEUR_1, TEMPS_ROULER_MS);
            avancerRobot(0, FREQUENCE_PWM_MOTEUR_2, TEMPS_ROULER_MS);
            avancerRobot(25, FREQUENCE_PWM_MOTEUR_2, TEMPS_ROULER_MS);
            avancerRobot(50, FREQUENCE_PWM_MOTEUR_2, TEMPS_ROULER_MS);
            avancerRobot(75, FREQUENCE_PWM_MOTEUR_2, TEMPS_ROULER_MS);
            avancerRobot(100, FREQUENCE_PWM_MOTEUR_2, TEMPS_ROULER_MS);
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