#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include <cmath>
#include "constants.h"


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
	Texture textureBackground = TextureHolder::getTexture("graphics/background_sheet.png");

	// prepare the horde
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;


	// 100 bullets should do
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 3.0f;
	// when was the fire button last pressed?
	Time lastPressed;


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

						numZombies = 20;
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

				if (state == State::PLAYING)
				{
					// reloading
					if (e.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// plenty of bullets. reload
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0)
						{
							// only a few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{
							// nothing here yet
						}
					}
				}

			}
			if (e.type == sf::Event::Closed)
				wnd.close();

			if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)
				wnd.close();
			
		}

		// Handle WASD and mouse click while playing
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

			// Fire a bullet
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					mouseScreenPosition = Mouse::getPosition(wnd);
					mouseWorldPosition = wnd.mapPixelToCoords(Mouse::getPosition(wnd), mainView);
					// pass the center of the player and crosshair to the shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;

					bulletsInClip--;
				}
			} // END FIre a bullet

		} // END WASD and mouse click while playing



		// Update frame
		if (state == State::PLAYING)
		{
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition(wnd);
			mouseWorldPosition = wnd.mapPixelToCoords(Mouse::getPosition(wnd), mainView);

			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());

			// make the view centre around the player
			mainView.setCenter(playerPosition);

			// loop through each zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// Update any bullets that are in fligtth
			for (int i = 0; i < 100; i++)
			{ 
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}
		}// END UPDATING SCENE



		if (state == State::PLAYING)
		{
			wnd.clear();


			wnd.setView(mainView);

			wnd.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; i++)
			{
				wnd.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					wnd.draw(bullets[i].getShape());
				}
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