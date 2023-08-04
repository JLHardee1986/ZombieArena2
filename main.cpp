#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"


using namespace sf;

int main()
{
	TextureHolder holder;
	
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;

	// get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = (float)VideoMode::getDesktopMode().width;
	resolution.y = (float)VideoMode::getDesktopMode().height;

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow wnd(VideoMode((unsigned int)800, (unsigned int)500)
		, "Zombie Arena", Style::Default, settings);

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

	
	// Generate the map
	VertexArray background;
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background_sheet.png");

	// prepare the horde
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;


	while (wnd.isOpen())
	{
		sf::Event e;
		while (wnd.pollEvent(e))
		{
			if (e.type == Event::KeyPressed)
			{
				// Pause the game while playing
				if (e.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (e.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					//reset the game clock timer
					clock.restart();
				}

				// start a new game while in GAME_OVER state
				else if (e.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{

				}

			}
			if (e.type == sf::Event::Closed)
				wnd.close();

			if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)
				wnd.close();



			// Handle the leveling up state
			if (state == State::LEVELING_UP)
			{
				// Handle the player LEVELING up
				if (e.key.code == Keyboard::Num1)
				{
					state = State::PLAYING;
				}
				if (e.key.code == Keyboard::Num2)
				{
					state = State::PLAYING;
				}
				if (e.key.code == Keyboard::Num3)
				{
					state = State::PLAYING;
				}
				if (e.key.code == Keyboard::Num4)
				{
					state = State::PLAYING;
				}
				if (e.key.code == Keyboard::Num5)
				{
					state = State::PLAYING;
				}
				if (e.key.code == Keyboard::Num6)
				{
					state = State::PLAYING;
				}

				if (state == State::PLAYING)
				{
					// Prepare the level
					// We will modify the next two lines after
					arena.width = 1500;
					arena.height = 1500;
					arena.left = 0;
					arena.top = 0;


					// pass the vertex array by reference
					// to the createBackground function
					int tileSize = createBackground(background, arena);

					// Spawn the player in the middle of the arena
					player.spawn(arena, resolution, tileSize);

					numZombies = 1000;
					if (zombies != nullptr) 
					{ 
						delete[] zombies;
						zombies = nullptr; 
					}

					zombies = createHorde(numZombies, arena);
					numZombiesAlive = numZombies;


					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

			} // end of LEVELING UP
		}
		// Handle WASD while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
		} // END WASD while playing

		// Update frame
		if (state == State::PLAYING)
		{
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = wnd.mapPixelToCoords(Mouse::getPosition(), mainView);

			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());

			// make the view centre around the player
			mainView.setCenter(player.getCenter());

			// loop through each zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
		}

		if (state == State::PLAYING)
		{
			wnd.clear();


			wnd.setView(mainView);

			wnd.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; i++)
			{
				wnd.draw(zombies[i].getSprite());
			}

			wnd.draw(player.getSprite());

			wnd.display();
		}
		if (state == State::LEVELING_UP)
		{
			wnd.clear();
			wnd.display();
		}
		if (state == State::PAUSED)
		{
			wnd.clear();
			wnd.display();
		}
		if (state == State::GAME_OVER)
		{
			wnd.clear();
			wnd.display();
		}

		
	}// end game loop
	
	delete[] zombies;

	return EXIT_SUCCESS;
}