#include "Bullet.h"

Bullet::Bullet()

	: m_bulletDistanceX(0), m_bulletDistanceY(0)
	, m_pos({ 0.f,0.f }), m_bulletShape()
	, m_inFlight(false), m_bulletSpeed(1000)
	, m_maxX(1300), m_minX(-2), m_maxY(800), m_minY(-2)
{
	m_bulletShape.setSize(sf::Vector2f(2, 2));
	m_bulletShape.setFillColor(sf::Color::Red);
}

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget)
{

	// keep track of the bullet
	m_inFlight = true;
	m_pos.x = startX;
	m_pos.y = startY;

	// Calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);


	// any gradient less than one needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	
	// Calculate the ratio between x and y
	float ratioXY = m_bulletSpeed / (1 + gradient);

	// Set the speed horizontally and vertically
	m_bulletDistanceY = ratioXY;
	m_bulletDistanceX = ratioXY * gradient;

	// Point the bullet in the right direction
	if (targetX < startX)

	{
		m_bulletDistanceX *= -1;
	}


	if (targetY < startY)

	{
		m_bulletDistanceY *= -1;
	}

	// set a max range of 1000 pixels
	float range = 1000;
	m_minX = startX - range;
	m_maxX = startX + range;
	m_minY = startY - range;
	m_maxY = startY + range;

	// position the bullet ready to be drawn

	m_bulletShape.setPosition(m_pos);

}

void Bullet::stop()
{
	m_inFlight = false;
}

bool Bullet::isInFlight()
{
	return m_inFlight;
}




sf::FloatRect Bullet::getPosition()
{
	return m_bulletShape.getGlobalBounds();
}

sf::RectangleShape Bullet::getShape()
{
	return m_bulletShape;
}

void Bullet::update(float elapsedTime)
{

	// Update the bullet position variables
	m_pos.x += m_bulletDistanceX * elapsedTime;
	m_pos.y += m_bulletDistanceY * elapsedTime;

	// move the bullet
	m_bulletShape.setPosition(m_pos);

	// has the bullet gone out of range?
	if (m_pos.x < m_minX || m_pos.x > m_maxX ||
		m_pos.y < m_minY || m_pos.y > m_maxY)
	{
		m_inFlight = false;
	}

}
