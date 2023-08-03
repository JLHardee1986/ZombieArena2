#include <SFML/Graphics.hpp>
#include "Player.h"
using namespace sf;

int main()
{
	
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;

	// get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow wnd(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen, settings);

	// create an sfml view for the main action
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	// here is our clock for timing everything
	Clock clock;

	// How long has the PLAYING state been active
	Time gameTimeTotal;

	Vector2f mouseWorldPosition;

	Vector2i mouseScreenPosition;

	Player player;

	IntRect arena;

	while (wnd.isOpen())
	{
		sf::Event e;
		while (wnd.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				wnd.close();

			if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)
				wnd.close();
		}
		wnd.clear();

		wnd.display();
	}
	
	return EXIT_SUCCESS;
}