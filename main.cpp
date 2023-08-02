#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow wnd(VideoMode(800, 600, 32), "ZombieArena", Style::Default, settings);
	
	while (wnd.isOpen())
	{
		sf::Event e;
		while (wnd.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				wnd.close();
		}
		wnd.clear();

		wnd.display();
	}
	
	return EXIT_SUCCESS;
}