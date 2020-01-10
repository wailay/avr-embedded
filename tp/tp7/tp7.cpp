#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<stdint.h>
#include <avr/interrupt.h>
#include "del.h"
#include "moteur.h"
#include "interrupt.h"
#include "minuterie.h"
#include "can.h"
#include "memoire_24.h"
#include "uart.h"

DEL del;
Interrupt b_int(0, ANY_EDGE);

ISR(TIMER1_COMPA_vect)
{
    del.allumerDEL(VERTE);
}

ISR(INT0_vect)
{
    _delay_ms(30);
    
    if (b_int.isButtonPressed()){
       del.allumerDEL(ROUGE);
    }else{
        del.allumerDEL(VERTE);
    }
     EIFR |= (1 << INTF0) ;
}

int main()
{
    
    // can conv;
    // uint8_t a = 0x1345;
    // unsigned char b;
    partirMinuterie1(7812);
    // arreterMinuterie1();
// for(;;){
    //On veux ADC0 donc MUX4:0 doit etre 0000, dou PA0 = 0 selon AVRlibc
    //Test can uart
    // uint8_t lectureCan = (conv.lecture(PA0) >> 2);

    // uart.transmissionUART(lectureCan);

    // Memoire24CXXX memoire;
    // char data[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    // char rx[sizeof(data)];
    // del.allumerDEL(ROUGE);
    // memoire.ecriture(0x00, (uint8_t *) data, sizeof(data));
    // _delay_ms(5);
    // //ecrire 0x00 a la fin de la chaine.
    // memoire.ecriture(0x00 + sizeof(data), 0xFF);
    // _delay_ms(5);
    // memoire.lecture(0x00, (uint8_t *) rx, sizeof(data) + 1);

    // for(unsigned int i =0; i < sizeof(data); i++)
    // {
    //    transmissionUART(rx[i]);
    // }
    
    // b = uart.receptionUART();


    // uart.transmissionUART(b);
// }
    
    for(;;){}
    
    return 0;
}
