#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<stdint.h>
#include"memoire_24.h"

bool checkIfEqual(char a[], char b[], int char_count){
    for(int i = 0; i<char_count;i++){
        if( a[i] != b[i] ){
            return 0;
        }
    }
    return 1;
}

int main(){
    DDRA = 0xff; // PORT A est en mode sortie.
    DDRB = 0xff; // PORT B est en mode sortie.
    DDRC = 0xff; // PORT C est en mode sortie.
    DDRD = 0xff; // PORT D est en mode sortie.
    const uint8_t adresse = 0x00;
    Memoire24CXXX Mem = Memoire24CXXX();
    char Poly[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E**M*O*N*T*R*E*A*L*";
    const int nb_caracteres = 44;
// "*P*O*L*Y*T*E*C*H*N*I*Q*U*E**M*O*N*T*R*E*A*L*";
// Si nb_caracteres dépasse 18, le programme arrête de fonctionner.
// 44 / 11 = 4 packets de 11 octets.
    Mem.ecriture(adresse,(uint8_t*)Poly,sizeof(Poly));
    char donneesSurEeprom[sizeof(Poly)];
    Mem.lecture(adresse,(uint8_t*)donneesSurEeprom,sizeof(Poly));
    if(checkIfEqual(Poly,donneesSurEeprom,nb_caracteres)){
    	for(;;){    
        PORTD = 0x01;
	    _delay_ms(10);
	    PORTD = 0x02;
	    _delay_ms(10);
    }
    }
    else 
    PORTD = 0x02;
    }

