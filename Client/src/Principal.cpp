#include "..\include\Principal.h"

void unistall(void *data)
{
    Serveur *tmp = static_cast<Serveur *>(data);
    tmp->desinstall();
}

Principal::Principal()
{
    images.load(param);
    serveur.load(&param, &Running, &progression, &Jouer, &Desinstaller);
    progression.load(&param, &images);
    Jouer.load(images.getBouton(0), images.getBouton(1), images.getBouton(2), images.getBouton(3), 0, NULL, (&Principal::Fonction_jouer), this, this);
    Desinstaller.load(images.getBouton(4), images.getBouton(5), images.getBouton(6), images.getBouton(7), 0, NULL, NULL, this, this);
    Desinstaller.setDonneeClique(&serveur);
    App.Create(sf::VideoMode(images.getFond().GetSize().x, images.getFond().GetSize().y, 32), param.getTitrePatcher(), sf::Style::Titlebar | sf::Style::Close);
    credit.SetText(L"Realise par Neckara pour le projet Last Dungeon, ALL RIGHT RESERVED");
    credit.SetSize(15);
    credit.SetPosition(images.getFond().GetSize().x - credit.GetRect().GetWidth(), images.getFond().GetSize().y - credit.GetSize() - 5);
    App.Draw(images.getFond());
    App.SetIcon(images.getIcone().GetWidth(), images.getIcone().GetHeight(), images.getIcone().GetPixelsPtr());
    App.Display();
    Running = true;
    App.SetActive(false);
    param.setMessage(0);
    sf::Thread Thread(&afficher, this);
    Thread.Launch();
    sf::Thread Majs_auto(&maj_auto, &serveur);
    Majs_auto.Launch();
    event();
    Thread.Wait();
    Majs_auto.Wait();
    serveur.Wait();
}

Principal::~Principal()
{
    //dtor
}

void Principal::event()
{
    int x, y, z;
   	while (Running)
	{
		sf::Event Event;
		while (App.GetEvent(Event))
		{
			if((Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape) || Event.Type == sf::Event::Closed)
			{
				Running = false;
				break;
			}
			if(Event.Type == sf::Event::MouseButtonPressed || Event.Type == sf::Event::MouseButtonReleased || Event.Type == sf::Event::MouseMoved)
			{
                if(Event.Type == sf::Event::MouseButtonPressed || Event.Type == sf::Event::MouseButtonReleased)
                {
                    x = Event.MouseButton.X;
                    y = Event.MouseButton.Y;
                    if(Event.Type == sf::Event::MouseButtonPressed)
                        z = 0;
                    else
                        z = 2;
                }
                else
                {
                    x = Event.MouseMove.X;
                    y = Event.MouseMove.Y;
                    z = 1;
                }
                Jouer.dessus(x, y, z);
                Desinstaller.dessus(x, y, z);
			}

		}
	}
}

void afficher(void *data)
{
    Principal *p = static_cast<Principal *>(data);
    p->affichage();
}

void Principal::affichage()
{
    App.SetFramerateLimit(30);
    while(Running)
    {
            App.Draw(images.getFond());
            int message = param.getMessage();
            if(message < nb_texte)
                App.Draw(param.getMessage(message));
            progression.Draw(App);
            App.Draw(Jouer.getImage());
            App.Draw(Desinstaller.getImage());
            App.Draw(credit);
            App.Display();
    }
}

void maj_auto(void *data)
{
    Serveur *maj = static_cast<Serveur *>(data);
    maj->majPatcher();
    //majJeux();
}

void Principal::Fonction_jouer(void *data)
{
    Running = false;
    system(param.getJeux().c_str());
}
