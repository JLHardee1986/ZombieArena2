#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include <cmath>
#include "constants.h"
#include "Pickup.h"
#include <sstream>



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
	RenderWindow wnd(VideoMode((unsigned int)1280, (unsigned int)720)
		, "Zombie Arena", Style::None, settings);

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

	wnd.setMouseCursorVisible(false);
	Sprite spriteCrosshair(TextureHolder::getTexture("graphics/crosshair.png"));
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// about the game
	int score = 0;
	int hiScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::getTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::getTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);
	// Load the font
	Font font;
	font.loadFromFile("fonts/DS-DIGI.TTF");
	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");
	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);

	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");
	// LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 200);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());
	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");
	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	// Debug HUD
	Text debugText;
	debugText.setFont(font);
	debugText.setCharacterSize(25);
	debugText.setFillColor(Color::White);
	debugText.setPosition(20.f, 220.f);
	std::ostringstream ss;

	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 1000;

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

						// Configure the pickups
						healthPickup.setArena(arena);
						ammoPickup.setArena(arena);

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

			spriteCrosshair.setPosition(mouseWorldPosition);

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

			// update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);


			// collision detection
			// Have any zombies been shot
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							// stop the bullet
							bullets[i].stop();

							// register the hit and see if it was a kill
							if (zombies[j].hit())
							{
								// if here, then the zombie was hit and killed with this shot
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numZombiesAlive--;

								// when all the zombies are dead (again)
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			} // End of zombies being shot

			// Have any zombies touched the player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{

					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
					}
				}
			}

			// Has the player touched health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}

			// Has the player touched ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
			}

			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			// Increment the number of frames since the previous update
			framesSinceLastHUDUpdate++;
			// re-calculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				// Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				// Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}// End HUD update

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

			// draw pickups, if currently spawned
			if (ammoPickup.isSpawned())
				wnd.draw(ammoPickup.getSprite());

			if (healthPickup.isSpawned())
				wnd.draw(healthPickup.getSprite());

			wnd.draw(spriteCrosshair);


			// HUD
			wnd.setView(hudView);

			wnd.draw(spriteAmmoIcon);
			wnd.draw(ammoText);
			wnd.draw(scoreText);
			wnd.draw(hiScoreText);
			wnd.draw(healthBar);
			wnd.draw(waveNumberText);
			wnd.draw(zombiesRemainingText);


			wnd.display();
		}
		if (state == State::LEVELING_UP)
		{
			wnd.clear();

			wnd.draw(spriteGameOver);
			wnd.draw(levelUpText);

			wnd.display();
		}
		if (state == State::PAUSED)
		{
			wnd.clear();
			wnd.draw(pausedText);
			wnd.display();
		}
		if (state == State::GAME_OVER)
		{
			wnd.clear();

			wnd.draw(spriteGameOver);
			wnd.draw(gameOverText);
			wnd.draw(scoreText);
			wnd.draw(hiScoreText);


			wnd.display();
		}

		
	}// end game loop
	
	delete[] zombies;

	return EXIT_SUCCESS;
}