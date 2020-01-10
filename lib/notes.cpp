/*
 * Noms des auteurs : Andrew Abdo, Aymeric Labrie, Youva Boutora et Wail Ayad  
 * Numero equipe : 1217
 * Description: Classe qui implemente le piezoelectrique pour generer des son a l'aide de la minuterie 0 CTC.
 * 
*/
#include "notes.h"

void playNote(uint8_t note)
{
    static const double FREQUENCES[] = {
        110,
        116.5409404,
        123.4708253,
        130.8127827,
        138.5913155,
        146.832384,
        155.5634919,
        164.8137785,
        174.6141157,
        184.9972114,
        195.997718,
        207.6523488,
        220,
        233.0818808,
        246.9416506,
        261.6255653,
        277.182631,
        293.6647679,
        311.1269837,
        329.6275569,
        349.2282314,
        369.9944227,
        391.995436,
        415.3046976,
        440,
        466.1637615,
        493.8833013,
        523.2511306,
        554.365262,
        587.3295358,
        622.2539674,
        659.2551138,
        698.4564629,
        739.9888454,
        783.990872,
        830.6093952,
        880};
    if (note >= 45 && note <= 81)
    {
        uint8_t ocrnx = F_CPU / (2 * 256 * FREQUENCES[note - 45]) - 1; //OCRnx = F_CPU/(2*N*freq_desiree) - 1
        startTimer0CTC(ocrnx);
    }
}

void playNote(uint8_t note, uint16_t duree_ms, uint8_t nRepetition)
{
    for (uint8_t i = 0; i < nRepetition; i++)
        playNote(note);
}

void stopNote()
{
    stopTimer0CTC();
}