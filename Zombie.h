#ifndef __ZOMBIE__
#define __ZOMBIE__

#include <SFML/Graphics.hpp>

class Zombie
{
private:
	// speed of each type
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	// toughness
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	// variance in speeds
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	sf::Vector2f m_position;

	sf::Sprite m_sprite;

	float m_speed;

	float m_health;

	bool m_isAlive;

public:
	bool hit();
	bool isAlive();

	void spawn(float startX, float startY, int type, int seed);

	sf::FloatRect getPosition();
	sf::Sprite getSprite();

	void update(float elapsedTime, sf::Vector2f playerLocation);

};

#endif
