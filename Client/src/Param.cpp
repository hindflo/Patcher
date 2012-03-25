
#include "../include/Param.h"

Param::Param()
{
    textes = new sf::String[nb_texte];
    Font = new sf::Font[nb_texte];
    message = nb_texte;
    //initialisation des variables
    version = v;
    std::string ligne;
    titre_patcher = "Patcher v." + version;
    icone = "";
    fond = "";
    std::ifstream fichier("param.txt", std::ios::in);
    if(fichier)
	{
	    //titre du patcher
	    lire_string(fichier, titre_patcher);
	    titre_patcher += version;
	    lire_string(fichier, icone);
        lire_string(fichier, fond);
        lire_string(fichier, serveur);
	    lire_string(fichier, majJeux);
        lire_string(fichier, VersionSpeciale);
        lire_string(fichier, Jeux);
        lire_string(fichier, fileVersion);
	    lire_string(fichier, majPatcher);
	    int i = 0;
	    while(i != 8)
	    {
	        lire_string(fichier, img_Bouton[i]);
	        lire_position(fichier, position[i][0], position[i][1]);
	        i++;
	    }
	    lire_string(fichier, barre);
        lire_position(fichier, xbarre, ybarre);
	    lire_string(fichier, bfond);
	    lire_position(fichier, xbfond, ybfond);
        i = 0;
        while(i != nb_texte)
        {
            lire_Texte(fichier, textes[i], Font[i]);
            i++;
        }
        fichier.close();
	}
	else
	{
        std::cerr << "Erreur lors de l'ouverture du fichier de param" << std::endl;
        exit(1);
	}
}

Param::~Param()
{
    delete[] (textes);
    delete[] (Font);
}

void Param::lire_string(std::ifstream &fichier, std::string &destination)
{
    fichier.ignore( std::numeric_limits<std::streamsize>::max(), ':');
    std::getline(fichier, destination);
    destination.erase(0,1);
}

void Param::lire_Texte(std::ifstream &fichier, sf::String &destination, sf::Font &myFont)
{
    std::string ligne;
    int x, y, z;
    lire_string(fichier, ligne);
    destination.SetText(traduire(ligne.c_str()));
    lire_position(fichier, x, y);
    destination.SetPosition(x, y);
    lire_string(fichier, ligne);
    if(ligne != "default" && !myFont.LoadFromFile(ligne))
    {
        std::cerr << "Erreur lors du chargement de la police '" << ligne << "'" << std::endl;
        myFont = sf::Font::GetDefaultFont();
    }
    else if(ligne == "default")
        myFont = sf::Font::GetDefaultFont();
    lire_int(fichier, x);
    destination.SetSize(x);
    lire_string(fichier, ligne);
    set_police(destination, ligne.c_str());
    lire_couleur(fichier, x, y, z);
    destination.SetColor(sf::Color(x, y, z));
    lire_int(fichier, x);
    destination.SetRotation(x);
}

std::string Param::getTitrePatcher()
{
    return titre_patcher;
}

std::string Param::getIcone()
{
    return icone;
}

std::string Param::getFond()
{
    return fond;
}

std::string Param::getServeur()
{
    return serveur;
}

std::string Param::getMajPatcher()
{
    return majPatcher;
}

std::string Param::getMajJeux()
{
    return majJeux;
}

std::string Param::getVersion()
{
    return version;
}

int Param::getMessage()
{
    return message;
}

std::string Param::getSousVersion()
{
    return VersionSpeciale;
}

sf::String Param::getMessage(int m)
{
    return textes[m];
}

wchar_t * Param::traduire(const char *test)
{
	int i = 0;
	int n = strlen(test);
	int j = 0;
	wchar_t * retour = new wchar_t[n+1];
	while(i != n)
	{
		if(test[i] < 0)
		{
			retour[j] = L'?';
			//std::cout << 256 + (int)test[i] + 256 + (int)test[++i] - 131 << std::endl;
		}
		else
		{
			retour[j] = test[i];
		}
		j++;
		i++;
	}
	retour[j] = L'\0';
	return retour;
}

void Param::setMessage(int m)
{
        message = m;
}

void Param::lire_position(std::ifstream &fichier, int & x, int & y)
{
    fichier.ignore( std::numeric_limits<std::streamsize>::max(), ':');
    fichier >> x >> y;
}

void Param::lire_int(std::ifstream &fichier, int & x)
{
    fichier.ignore( std::numeric_limits<std::streamsize>::max(), ':');
    fichier >> x;
}

void Param::set_police(sf::String &destination, const char *ligne)
{
    int i = 0;
    long unsigned int style = sf::String::Regular;
    while(ligne[i] != '\0')
    {
        switch(ligne[i])
        {
            case 's' :
            case 'u' :
            case 'S' :
            case 'U' :
                style |=  sf::String::Underlined;
            break;
            case 'b' :
            case 'g' :
            case 'B' :
            case 'G' :
                style |=  sf::String::Bold;
            break;
            case 'n' :
            case 'd' :
            case 'N' :
            case 'D' :
                style = sf::String::Regular;
            break;
            case 'i' :
            case 'I' :
                style |=  sf::String::Italic;
            break;
        }
        i++;
        while(ligne[i] != '|' && ligne[i] != '\0')
        {
            i++;
        }
        if(ligne[i] != '\0')
        {
            i++;
            if(ligne[i] == ' ')
                i++;
        }
    }
    destination.SetStyle(style);
}

void Param::lire_couleur(std::ifstream &fichier, int &x, int &y, int &z)
{
    fichier.ignore( std::numeric_limits<std::streamsize>::max(), ':');
    fichier >> x >> y >> z;
}

std::string Param::getFileVersion()
{
    return fileVersion;
}

std::string Param::getBfond()
{
    return bfond;
}
std::string Param::getBarre()
{
    return barre;
}

int Param::getXB()
{
    return xbfond + xbarre;
}
int Param::getYB()
{
    return ybfond + ybarre;
}
int Param::getXF()
{
    return xbfond;
}
int Param::getYF()
{
    return ybfond;
}

sf::String Param::getPourcent()
{
    return textes[nb_texte - 2];
}
sf::String Param::getTitre()
{
    return textes[nb_texte - 3];
}
sf::String Param::getFichiers()
{
    return textes[nb_texte - 1];
}

std::string Param::getJeux()
{
    return Jeux;
}

std::string Param::getBouton(int x)
{
    return img_Bouton[x];
}

int Param::getPosition(int x, int y)
{
    return position[x][y];
}
