#ifndef __BULLET__
#define __BULLET__

#include <SFML/Graphics.hpp>

class Bullet
{


	// Where is the bullet?
	sf::Vector2f m_pos;

	// What each bullet looks like
	sf::RectangleShape m_bulletShape;

	// is this bullet currently whizzing through the air
	bool m_inFlight = false;

	// how fast does a bullet travel?
	float m_bulletSpeed = 1000;

	// What fraction of 1 pixel does the bullet travel,
	//  Horizontally and vertically each frame?
	//  These values will be dervied from bulletSpeed
	float m_bulletDistanceX;
	float m_bulletDistanceY;

	// some boundaries so the bullet doesn't fly forever
	float m_maxX;
	float m_minX;
	float m_maxY;
	float m_minY;

public:

	//ctor 
	Bullet();

	// stop the bullet
	void stop();

	bool isInFlight();

	// launch a new bullet
	void shoot(float startX, float startY, float targetX, float targetY);

	// Tell the calling code where the bullet is in the world
	sf::FloatRect getPosition();

	// Return the actual shape (for drawing)
	sf::RectangleShape getShape();

	// Update the bullet each frame
	void update(float elapsedTime);
};

#endif