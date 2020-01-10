#include "del.h"
#include "uart.h"
#include "moteur.h"
#include "commun.h"
#include "debug.h"
#include "notes.h"
#include "memoire_24.h"
#include "interpreteur.h"
DEL del;

void start_sequence(){
        playNote(45);
        playNote(34);
        playNote(65);
        playNote(76);
        stopNote();
        del.allumerDEL(ROUGE,250);
        del.allumerDEL(OFF, 250);
        del.allumerDEL(ROUGE,250);
        del.allumerDEL(OFF, 250);
        del.allumerDEL(VERTE,250);
        del.allumerDEL(OFF, 250);
        del.allumerDEL(VERTE,250);
}
void darude_sandstorm(){
    playNote(71, 150, 7);
    playNote(64, 150, 7);
    playNote(62, 150, 6);
    playNote(69, 150, 2);
    playNote(71, 150, 5);
    playNote(71, 150, 7);
    playNote(64, 150, 3);
    playNote(71, 150, 11);
    playNote(64, 150, 2);
    playNote(71, 150, 12);
    playNote(64, 150, 7);
    playNote(62, 150, 7);
    playNote(69, 150, 2);
    playNote(71, 150, 12);
    playNote(64, 150, 2);
    playNote(71, 150, 12);
    playNote(64, 150, 2);
    playNote(71, 150, 2);
}

uint8_t find_start_program(Memoire24CXXX& mem, uint8_t byte_code_size)
{

    start_sequence();

    uint8_t programCounter = 1; //apres find byte code size
    uint8_t instruction[2];
     while (programCounter < byte_code_size)
    {
        mem.lecture(programCounter, (uint8_t *) instruction, 2);
        if (instruction[0] == dbt_bc)
            break;
    }
    return programCounter;
}

uint8_t find_byte_code_size(Memoire24CXXX& mem){
    uint8_t byte_code_size = 0;
    mem.lecture(uint8_t(0), &byte_code_size); // trouver byte_code_size
    return byte_code_size;
}

int main()
{   
    
    Memoire24CXXX mem;
    Interpreteur interpreteur;
    uint8_t byte_code_size, programCounter, instruction[2];
    byte_code_size = find_byte_code_size(mem);
    programCounter = find_start_program(mem, byte_code_size);
    interpreteur.setProgramCounter(programCounter);
    /********************
     *    MAIN LOOP     *
     * ******************/
    //Apres avoir trouver le debut d'instruction, lire la memoire a ladresse pointee par PC.
    programCounter = interpreteur.getPC();

    while ((programCounter < byte_code_size) && !interpreteur.terminated())
    {
        
        mem.lecture(programCounter, (uint8_t *) instruction, 2);
        interpreteur.parse_bytcode(instruction);
        programCounter = interpreteur.getPC();
    }
    
    return 1;

}