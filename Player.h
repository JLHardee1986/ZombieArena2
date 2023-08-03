#ifndef __PLAYER__
#define __PLAYER__

#include <SFML/Graphics.hpp>

class Player
{
private:
	const int START_SPEED = 200;
	const int START_HEALTH = 100;
	// Where is the player
	sf::Vector2f m_position;
	// Of course, we will need a sprite
	sf::Sprite m_sprite;
	// And a texture
	// !!Watch this space – Interesting changes here soon!!
	sf::Texture m_texture;
	// What is the screen resolution
	sf::Vector2f m_resolution;
	// What size is the current arena
	sf::IntRect m_arena;
	// How big is each tile of the arena
	int m_tileSize;
	// Which direction(s) is the player currently moving in
	bool m_upPressed;
	bool m_downPressed;
	bool m_leftPressed;
	bool m_rightPressed;
	// How much health has the player got?
	int m_health;
	// What is the maximum health the player can have
	int m_maxHealth;
	// When was the player last hit
	sf::Time m_lastHit;
	// Speed in pixels per second
	float m_speed;
	// All our public functions will come next
	// All our public functions will come next
public:
	Player();
	void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);
	// Call this at the end of every game
	void resetPlayerStats();

	// Handle the player getting hit by a zombie
	bool hit(sf::Time timeHit);
	// How long ago was the player last hit
	sf::Time getLastHitTime();
	// Where is the player
	sf::FloatRect getPosition();
	// Where is the center of the player
	sf::Vector2f getCenter();
	// What angle is the player facing
	float getRotation();
	// Send a copy of the sprite to the main function
	sf::Sprite getSprite();
	// The next four functions move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	// Stop the player moving in a specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	// We will call this function once every frame
	void update(float elapsedTime, sf::Vector2i mousePosition);
	// Give the player a speed boost
	void upgradeSpeed();
	// Give the player some health
	void upgradeHealth();
	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);
	// How much health has the player currently got?
	int getHealth();
};

#endif
