#ifndef PARAM_H
#define PARAM_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <SFML/Graphics.hpp>
#include <wchar.h>

const std::string v = "0";
const int nb_texte = 13;

class Param
{
    public:
        Param();
        ~Param();
        std::string getTitrePatcher();
        std::string getIcone();
        std::string getFond();
        std::string getServeur();
        std::string getMajPatcher();
        std::string getVersion();
        std::string getMajJeux();
        std::string getSousVersion();
        std::string getFileVersion();
        std::string getBfond();
        std::string getBarre();
        std::string getJeux();
        int getMessage();
        sf::String getMessage(int m);
        void setMessage(int m);
        int getXB();
        int getYB();
        int getXF();
        int getYF();
        sf::String getPourcent();
        sf::String getTitre();
        sf::String getFichiers();
        wchar_t * traduire(const char *test);
        std::string getBouton(int x);
        int getPosition(int x, int y);
    protected:
    private:
        std::string version;
        std::string fileVersion;
        std::string titre_patcher;
        std::string icone;
        std::string fond;
        std::string serveur;
        std::string majJeux;
        std::string VersionSpeciale;
        std::string Jeux;
        std::string majPatcher;
        std::string bfond;
        std::string barre;
        int xbfond;
        int ybfond;
        int xbarre;
        int ybarre;
        int message;
        sf::String *textes;
        sf::Font *Font;
        void lire_string(std::ifstream &fichier, std::string &destination);
        void lire_Texte(std::ifstream &fichier, sf::String &destination, sf::Font &myFont);
        void lire_position(std::ifstream &fichier, int & x, int & y);
        void lire_int(std::ifstream &fichier, int & x);
        void set_police(sf::String &destination, const char * ligne);
        void lire_couleur(std::ifstream &fichier, int &x, int &y, int &z);
        std::string img_Bouton[8];
        int position[8][2];
};

#endif // PARAM_H
