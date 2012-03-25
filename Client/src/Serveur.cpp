#include "../include/Serveur.h"

Serveur::Serveur()
{
    isDesinstall = false;
}

void Serveur::load(Param *p, bool *run, Progression *prog, Bouton <void *, void *,void (*)(void *),void (Principal::*)(void *), Principal *, Principal *> * J, Bouton <void *, void *,void (*)(void *), void (Principal::*)(void *),Principal *,Principal *> * D)
{
    Jouer = J;
    Desinstaller = D;
    param = p;
    progression = prog;
    Http.SetHost(param->getServeur());
    running = run;
    Request.SetMethod(sf::Http::Request::Get);
    Patcher = param->getMajPatcher() + "&version=" + param->getVersion();
    std::ifstream fichier(param->getFileVersion().c_str());
    fichier >> vers;
    if(fichier.fail()) vers = "0";
    Jeux = param->getMajJeux() + "&version=" + vers + "&sous-version=" + param->getSousVersion();
}

Serveur::~Serveur()
{
    //dtor
}


std::string Serveur::reception()
{
	return Page.GetBody();
}

bool Serveur::connexion(std::string chemin)
{
	Request.SetURI(chemin);
	Page = Http.SendRequest(Request);
	if(Page.GetStatus() != sf::Http::Response::Ok)
	{
		std::cerr << "Le serveur a retourné le code d'erreur : " << Page.GetStatus() << std::endl;
		return false;
	}
	return true;
}

void Serveur::majPatcher()
{
    if(connexion(Patcher))
    {
       maj(2);
    }
    else
    {
        param->setMessage(1);//serveur Patcher indisponible
        sf::Sleep(3);
    }
    param->setMessage(5);
    if(connexion(Jeux))
    {
         maj(6);
         param->setMessage(9);
    }
    else
        param->setMessage(8);
    Jouer->setActive(true);
    Desinstaller->setActive(true);
}

void Serveur::action(std::stringstream &fichier,std::string &ligne, std::string &arg)
{
    std::string destination;
    if(ligne == "redemarrer")
    {
        (*running) = false;
        std::string tmp = "start prePatcher.exe" + arg + " &";//pour linux, remplacer "start " par "./"
        system(tmp.c_str());
        return;
    }
    std::getline(fichier, destination);
    if(destination.find("./") != -1)
	{
	    std::cout << destination << std::endl;
        param->setMessage(4);
		sf::Sleep(3);
		exit(1);
    }
    if(ligne == "nouveau dossier")
    {
        //uniquement pour windows je crois
        CreateDirectory(destination.c_str(),NULL);
        //pour Linux? mkdir(destination.c_str());
    }
    else if(ligne == "supprimer dossier")
    {
        //Le répertoire doit être vide
        destination = "rmdir " + destination;
        system(destination.c_str());
        //rmdir(destination.c_str());
    }
    else if(ligne == "supprimer dossier Patcher")
    {
        arg += " supprimer_dossier " + destination;
    }
    else if(ligne == "supprimer")
    {
        remove(destination.c_str());
    }
    else if(ligne == "supprimer Patcher")
    {
        arg += " supprimer " + destination;
    }
    else if(ligne == "deplacer")
    {
        std::string tmp;
        std::getline(fichier, tmp);
        MoveFile(destination.c_str(), tmp.c_str());
    }
    else if(ligne == "deplacer Patcher")
    {
        std::string tmp;
        std::getline(fichier, tmp);
        arg += " deplacer " + destination + " " + tmp;
    }
    else if(ligne == "Telecharger" || ligne == "Telecharger Patcher")
    {
        std::string tmp;
        std::getline(fichier, tmp);
        connexion(destination);
        if(ligne == "Telecharger patcher")
        {
            tmp += "_bis";
            arg += " Patcher " + tmp;
        }
        std::ofstream ecrire(tmp.c_str());
        ecrire << Page.GetBody();
		ecrire.close();
    }
}

void Serveur::maj(int mess)
{
    int nb_instruction;
    std::stringstream fichier(Page.GetBody());
    std::string ligne;
    std::string arg;
    std::getline(fichier, ligne);
    if(ligne == "maj dispo")
        param->setMessage(mess);
    else if(ligne == "no maj dispo")
        param->setMessage(mess+1);
    fichier >> nb_instruction;
    progression->renit(nb_instruction, "Màj");
    std::getline(fichier, ligne);
    while(std::getline(fichier, ligne))
    {
        if(!*running) return;
        action(fichier, ligne, arg);
        progression->Progress();
    }
}

void Serveur::desinstall()
{
    sf::Thread *test= new sf::Thread(&thread_unistall, this);
    isDesinstall = true;
    Jouer->setActive(false);
    Desinstaller->setActive(false);
    test->Launch();
}

void Serveur::desinstall2()
{
    std::ifstream file("uninstall.txt");
    if(!file.fail())
    {
        std::string tmp;
        file >> tmp;
        std::stringstream fichier(tmp);
        int nb_instruction;
        std::string ligne;
        std::string arg;
        fichier >> nb_instruction;
        progression->renit(nb_instruction, "Désinstallation");
        std::getline(fichier, tmp);
        while(std::getline(fichier, tmp))
        {
            if(!*running) return;
            action(fichier, tmp, arg);
            progression->Progress();
        }
    }
    isDesinstall = false;
}

void thread_unistall(void *data)
{
    Serveur *tmp = static_cast<Serveur *>(data);
    tmp->desinstall2();
}

void Serveur::Wait()
{
    if(!isDesinstall)
        return;
    system("pause");
    test->Wait();
    system("pause");
    delete (test);
}
