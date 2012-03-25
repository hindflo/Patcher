#ifndef SERVEUR_H
#define SERVEUR_H

#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "Param.h"
#include "Progression.h"
#include "Bouton.h"

void thread_unistall(void *data);

class Principal;

class Serveur
{
    private:
        bool isDesinstall;
        sf::Thread *test;
        sf::Http Http;
		sf::Http::Request Request;
		sf::Http::Response Page;
		bool connexion(std::string chemin);
		std::string Patcher;
		std::string Jeux;
		bool *running;
		Param *param;
		Progression *progression;
		std::string vers;
		void action(std::stringstream &fichier,std::string &ligne, std::string &arg);
		void maj(int mess);

    public:
        Bouton <void *, void *,void (*)(void *),void (Principal::*)(void *), Principal *, Principal *> *Jouer;
		Bouton <void *, void *,void (*)(void *),void (Principal::*)(void *), Principal *, Principal *> *Desinstaller;
        void Wait();
        void desinstall2();
        void load(Param *p, bool *run, Progression *prog, Bouton <void *, void *,void (*)(void *),void (Principal::*)(void *), Principal *, Principal *>*J, Bouton <void *, void *,void (*)(void *),void (Principal::*)(void *), Principal *, Principal *> *D);
        void desinstall();
        Serveur();
        ~Serveur();
        std::string reception();
        void majPatcher();
    protected:

};

#endif // SERVEUR_H
