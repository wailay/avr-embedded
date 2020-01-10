#ifndef ROBOT_H
#define ROBOT_H

#include "headers.h"
#include "commun.h"
#include "moteur.h"
#include "del.h"
#include "notes.h"
#include "minuterie.h"
#include "display.h"
#include "notes.h"

enum Section {COULOIR, MUR, BOUCLES, COUPURES, FIN};
enum Transition {COULOIR_MUR, MUR_BOUCLES, BOUCLES_COUPURES, COUPURES_COULOIR};

class Robot
{
public:
    Robot();
    void setSectionInitial(Section Section);
    void afficherSection();
    void effectuerRoutineDebut();
    void changerEtat();
    void changerEtat(Section section); // Pour debug
    void effectuerEtat();
    bool estTerminee();
    
private:
    void effectuerCouloir_();
    void effectuerMur_();
    void effectuerBoucles_();
    void effectuerCoupures_();
    void effectuerFin_();
    void effectuerTransition_(Transition transition);
    Section sectionInitial_; 
    Section section_;
    Moteur moteur_;
    DEL del_;
    Display display_;
    bool estTerminee_;
    uint8_t vitesse_;
};
#endif // ROBOT_H