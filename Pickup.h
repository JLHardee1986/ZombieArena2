#ifndef __PICKUP__
#define __PICKUP__

#include <SFML/Graphics.hpp>

class Pickup
{
	const int HEALTH_START = 50;
	const int AMMO_START = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	sf::Sprite m_sprite;

public:
	// the arena is exists in
	sf::IntRect m_arena;

	// how much is the pickup worth?
	int m_value;

	// type of pickup
	// 1 = health, 2 = ammo
	int m_type;

	// Handle spawning and disappearing
	bool m_spawned;
	float m_secondsSinceSpawn;
	float m_secondsSinceDespawn;
	float m_secondsToLive;
	float m_secondsToWait;

public:
	
	Pickup(int type);

	void setArena(sf::IntRect arena);

	void spawn();

	// Check the position of a pickup
	sf::FloatRect getPosition();

	sf::Sprite getSprite();

	void update(float elapsedTime);

	bool isSpawned();

	int gotIt();

	void upgrade();
};
#endif

