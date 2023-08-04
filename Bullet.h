#ifndef __BULLET__
#define __BULLET_

#include <SFML/Graphics.hpp>

class Bullet
{
private:
	sf::Vector2f m_position;

	sf::RectangleShape m_bulletShape;

	bool m_inFlight = false;

	float m_bulletSpeed = 1000.f;

	// fraction of one pixel bullet travels each frame;
	float m_bulletDistanceX;
	float m_bulletDistanceY;

	// some boundaries so the bullet doesn't fly forever
	float m_maxX;
	float m_minX;
	float m_maxY;
	float m_minY;

public:
	Bullet();

	void stop();

	bool isInFlight();

	void shoot(float startX, float startY, float xTarget, float yTarget);

	sf::FloatRect getPosition();

	sf::RectangleShape getShape();

	void update(float elapsedTime);
};

#endif
