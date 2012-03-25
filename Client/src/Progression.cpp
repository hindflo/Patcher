#include "../include/Progression.h"

Progression::Progression()
{
    active = false;
}

Progression::~Progression()
{
    //dtor
}

bool Progression::isActive()
{
    return active;
}

void Progression::setActive(bool tmp)
{
    active = tmp;
}

void Progression::Draw(sf::RenderWindow &App)
{
    if(active)
    {
        App.Draw(img->getBFond());
        //TODO barre
        App.Draw(img->getBarre());
        App.Draw(pourcent);
        App.Draw(titre);
        App.Draw(nbfile);
    }
}

void Progression::load(Param *param, Images *images)
{
    par = param;
    img = images;
    pourcent = par->getPourcent();
    titre = par->getTitre();
    nbfile =  par->getFichiers();
    int x = img->getBFond().GetPosition().x;
    int y = img->getBFond().GetPosition().y;
    pourcent.SetPosition(pourcent.GetPosition().x + x, pourcent.GetPosition().y + y);
    titre.SetPosition(titre.GetPosition().x + x, titre.GetPosition().y + y);
    nbfile.SetPosition(nbfile.GetPosition().x + x, nbfile.GetPosition().y + y);
}

void Progression::renit(int nbInstructions, std::string title)
{
    nb_fait = 0;
    nb_max = nbInstructions;
    nb_pourcent = 0;
    active = true;
    titre.SetText(par->traduire(title.c_str()));
}

void Progression::Progress()
{
    nb_fait++;
    if(nb_fait == nb_max)
    {
        active = false;
        return;
    }
    nb_pourcent = (float)(nb_fait*10000/nb_max)/100;
    std::ostringstream ossib;
    ossib << nb_pourcent << "%";
    pourcent.SetText(ossib.str());
    std::ostringstream ossi;
    ossi << nb_fait << "/" << nb_max;
    nbfile.SetText(ossi.str());
    img->redimentionner((float)nb_fait/nb_max);
}
