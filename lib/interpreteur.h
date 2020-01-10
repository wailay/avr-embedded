/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description : Interpreteur du TP9
 * 
 */

#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#define dbt_bc 0b00000001
#define att_bc 0b00000010
#define dal_bc 0b01000100
#define det_bc 0b01000101
#define sgo_bc 0b01001000
#define sar_bc 0b00001001
#define mar_bc 0b01100000
#define mar2_bc 0b01100001
#define mav_bc 0b01100010
#define mre_bc 0b01100011
#define trd_bc 0b01100100
#define trg_bc 0b01100101
#define dbc_bc 0b11000000
#define fbc_bc 0b11000001
#define fin_bc 0b11111111

#include "headers.h"
#include "del.h"
#include "uart.h"
#include "moteur.h"
#include "commun.h"
#include "notes.h"
#include "memoire_24.h"

class Interpreteur
{
public:
    Interpreteur();
    void parse_bytcode(uint8_t byte_code[]);
    uint8_t getPC();
    void setProgramCounter(uint8_t programCounter);
    bool terminated();

private:
    void executerByteCode(uint8_t code, uint8_t operande);
    DEL del_;
    Moteur moteurs_;
    uint8_t instruction_[2];
    uint8_t programCounter_; //Pointeur de programme
    uint8_t byte_code_size_;
    uint8_t debutBoucle_; //Pointeur qui se rappele du debut de la loop
    uint8_t compteurBoucle_;
    bool isTerminated_;
};

#endif