#include "Bullet.h"

Bullet::Bullet()
{


	m_bulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::stop()
{
	m_inFlight = false;
}

bool Bullet::isInFlight()
{
	return m_inFlight;
}

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget)
{
	// track the bullet
	m_inFlight = true;
	m_position.x = startX;
	m_position.y = startY;

	// calculate the gradient of the flight path
	float gradient = (startX - xTarget) / (startY - yTarget);

	// any gradient less than one needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	// calculate the ratio between x and y
	float ratioXY = m_bulletSpeed / (1 + gradient);

	m_bulletDistanceY = ratioXY;
	m_bulletDistanceX = ratioXY * gradient;

	// point bullet
	if (xTarget < startX)
	{
		m_bulletDistanceX *= -1;
	}

	// point bullet
	if (yTarget < startY)
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
	m_bulletShape.setPosition(m_position);

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
	// update the bullet position variables
	m_position.x += m_bulletDistanceX * elapsedTime;
	m_position.y += m_bulletDistanceY * elapsedTime;

	// move the bullet
	m_bulletShape.setPosition(m_position);

	// Has the bullet gone out of range?
	if (m_position.x < m_minX || m_position.x > m_maxX || m_position.y < m_minY || m_position.y > m_maxY )
	{
		m_inFlight = false;
	}

}
