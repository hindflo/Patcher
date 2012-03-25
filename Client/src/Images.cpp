#include "..\include\Images.h"

Images::Images()
{

}

void Images::load(Param &param)
{
    if (!Fond.LoadFromFile(param.getFond()))
	{
		std::cerr << param.getFond() << " non trouvé\n" << std::endl;
		exit(1);
	}

    if (!Icone.LoadFromFile(param.getIcone()))
	{
		std::cerr << param.getIcone() << " non trouvé\n" << std::endl;
		exit(1);
	}
    if (!BFond.LoadFromFile(param.getBfond()))
	{
		std::cerr << param.getBfond() << " non trouvé\n" << std::endl;
		exit(1);
	}
	if (!Barre.LoadFromFile(param.getBarre()))
	{
		std::cerr << param.getBarre() << " non trouvé\n" << std::endl;
		exit(1);
	}

	SFond.SetImage(Fond);
	SBarre.SetImage(Barre);
	SBFond.SetImage(BFond);
	SBarre.SetPosition(param.getXB(), param.getYB());
	SBFond.SetPosition(param.getXF(), param.getYF());
	taille =  SBarre.GetSize().x;
	int i= 0;
	while(i != 8)
	{
        if (!img_Bouton[i].LoadFromFile(param.getBouton(i)))
        {
            std::cerr << param.getBouton(i) << " non trouvé\n" << std::endl;
            exit(1);
        }
        Bouton[i].SetImage(img_Bouton[i]);
        Bouton[i].SetPosition(param.getPosition(i, 0), param.getPosition(i, 1));
	    i++;
	}
}

Images::~Images()
{
    //dtor
}

void Images::redimentionner(float x)
{
    SBarre.SetSubRect(sf::IntRect(0, 0, taille*x, SBarre.GetSize().y));
}

sf::Sprite Images::getFond()
{
    return SFond;
}

sf::Image Images::getIcone()
{
    return Icone;
}

sf::Sprite Images::getBFond()
{
    return SBFond;
}

sf::Sprite Images::getBarre()
{
    return SBarre;
}

sf::Sprite Images::getBouton(int x)
{
    return Bouton[x];
}
