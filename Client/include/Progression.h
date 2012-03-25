#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <SFML/Graphics.hpp>
#include "images.h"
#include "param.h"
#include <sstream>

class Progression
{
    public:
        Progression();
        ~Progression();
        bool isActive();
        void setActive(bool tmp);
        void Draw(sf::RenderWindow &App);
        void load(Param *param, Images *images);
        void renit(int nbInstructions, std::string titre);
        void Progress();
    protected:
    private:
        sf::String pourcent;
        sf::String titre;
        sf::String nbfile;
        Param *par;
        Images *img;
        bool active;
        int nb_fait;
        int nb_max;
        float nb_pourcent;
};

#endif // PROGRESSION_H
