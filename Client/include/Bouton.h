#ifndef BOUTON_H
#define BOUTON_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
class Bouton
{
    public:
        void load(sf::Sprite desactive, sf::Sprite normal, sf::Sprite ionclur, sf::Sprite ionclick, int etat, FONCTION1 fonction_dessus = NULL, FONCTION2 fonction_clique = NULL, CLASSE1 temp_c1 = NULL, CLASSE2 temp_c2 = NULL);
        Bouton();
        ~Bouton();
        sf::Sprite getImage();
        bool dessus(int x, int y, int type);//0 : click, 1: dessus 2 : relachements
        void setDonneeClique(DONNEE2 donnees);
        void setDonneeDessus(DONNEE1 donnees);
        void setActive(bool active);
    protected:
    private:
        CLASSE1 c1;
        CLASSE2 c2;
        FONCTION1 fonction_dessus;
        FONCTION2 fonction_clique;
        sf::Sprite Images[4];
        //0 : désactive
        //1 : normal
        //2 : onblur
        //3 : onclick
        DONNEE1 donnees_dessus;
        DONNEE2 donnees_clique;
        int etat;
};


template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2, CLASSE1, CLASSE2>::Bouton()
{
}

template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
void Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2,CLASSE1,CLASSE2>::load(sf::Sprite desactive, sf::Sprite normal, sf::Sprite ionblur, sf::Sprite ionclick, int etats, FONCTION1 temp_fonction_dessus, FONCTION2 temp_fonction_clique, CLASSE1 temp_c1, CLASSE2 temp_c2)
{
    etat = etats;
    Images[0] = desactive;
    Images[1] = normal;
    Images[2] = ionblur;
    Images[3] = ionclick;
    fonction_dessus = temp_fonction_dessus;
    fonction_clique = temp_fonction_clique;
    c1 = temp_c1;
    c2 = temp_c2;
}

template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2, CLASSE1, CLASSE2>::~Bouton()
{
    //dtor
}

template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
sf::Sprite Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2, CLASSE1, CLASSE2>::getImage()
{
        return Images[etat];
}

template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
bool Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2, CLASSE1, CLASSE2>::dessus(int x, int y, int type)
{
    if(etat == 0)//bouton désactivé
        return false;
    x -= Images[etat].GetPosition().x;
    y -= Images[etat].GetPosition().y;
    if(x < 0 || x >= Images[etat].GetSize().x || y < 0 || y >= Images[etat].GetSize().y)
    {
        etat = 1;
        return false;
    }

    if(Images[etat].GetPixel(x, y).a == 0)
    {
        etat = 1;
        return false;
    }
    if(type == 0)//clique
    {
        std::cout << "??" << std::endl;
        etat = 3;
        if(fonction_clique != NULL)
        {
            std::cout << "!!" << std::endl;
            (c1->*fonction_clique)(donnees_clique);
            //(c1->Fonction_jouer)(donnees_clique);
        }
    }
    if(type == 2)//relaché
    {
        etat = 2;
    }
    if(type == 1)//dessus
    {
        etat = 2;
        if(fonction_dessus != NULL)
            fonction_dessus(donnees_dessus);
    }
    return true;
}

template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
void Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2, CLASSE1, CLASSE2>::setDonneeClique(DONNEE2 donnees)
{
    donnees_clique = donnees;
}
/*
template <typename DONNEE2, typename FONCTION1, typename FONCTION2>
void Bouton<int, DONNEE2, FONCTION1, FONCTION2>::setDonneeClique(DONNEE2 donnees)
{
    std::cerr << "Cette fonction ne prend pas d'argument" << std::endl;
}
*/
template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
void Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2, CLASSE1, CLASSE2>::setDonneeDessus(DONNEE1 donnees)
{
    donnees_dessus = donnees;
}

template<typename DONNEE1, typename DONNEE2, typename FONCTION1, typename FONCTION2, typename CLASSE1, typename CLASSE2>
void Bouton<DONNEE1, DONNEE2, FONCTION1, FONCTION2, CLASSE1, CLASSE2>::setActive(bool active)
{
    if(active && etat == 0)
        etat = 1;
    else if(!active)
        etat = 0;
}


#endif // BOUTON_H
