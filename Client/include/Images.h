#ifndef IMAGES_H
#define IMAGES_H

#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Param.h"

class Images
{
    public:
        Images();
        void load(Param &param);
        ~Images();
        sf::Sprite getFond();
        sf::Image getIcone();
        sf::Sprite getBFond();
        sf::Sprite getBarre();
        void redimentionner(float x);
        sf::Sprite getBouton(int x);
    protected:
    private:
        sf::Sprite Bouton[8];
        sf::Image img_Bouton[8];
        int taille;
        sf::Image Fond;
        sf::Sprite SFond;
        sf::Image Icone;
        sf::Image BFond;
        sf::Image Barre;
        sf::Sprite SBFond;
        sf::Sprite SBarre;
};

#endif // IMAGES_H
