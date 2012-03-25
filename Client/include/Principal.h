#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Images.h"
#include "Serveur.h"
#include "Param.h"
#include "Progression.h"
#include "Bouton.h"
#include <string>


void afficher(void *data);
void maj_auto(void *data);
void unistall(void *data);

class Principal
{
    public:
        Principal();
        ~Principal();
        void affichage(void);
        Bouton <void *, void *,void (*)(void *), void (Principal::*)(void *), Principal *, Principal *>Jouer;
        Bouton <void *, void *,void (*)(void *), void (Principal::*)(void *), Principal *, Principal *>Desinstaller;
        void Fonction_jouer(void *data);
    protected:
    private:
        sf::String credit;
        bool Running;
        sf::RenderWindow App;
        Param param;
        Images images;
        Serveur serveur;
        Progression progression;
        void event();
};
#endif // PRINCIPAL_H
