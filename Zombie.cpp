#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

bool Zombie::hit()
{
	m_health--;

	if (m_health < 0)
	{
		// dead
		m_isAlive = false;
		m_sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));

		return true;
	}

	// injured but not yet dead
	return false;
}

bool Zombie::isAlive()
{
	return m_isAlive;
}

void Zombie::spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
		case 0: // bloater
			m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/bloater.png"));

			m_speed = BLOATER_SPEED;
			m_health = BLOATER_HEALTH;
			break;
		case 1: // chaser
			m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/chaser.png"));

			m_speed = CHASER_SPEED;
			m_health = CHASER_HEALTH;
			break;
		case 2: // crawler
			m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/crawler.png"));

			m_speed = CRAWLER_SPEED;
			m_health = CRAWLER_HEALTH;
			break;
		default:
			break;
	}

	//  modify speed
	srand((int)time(0) * seed);

	// somewhere between 80 and 100
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;

	// Express this as a fraction of 1
	modifier /= 100;
	m_speed *= modifier;

	// Initialize its location
	m_position.x = startX;
	m_position.y = startY;

	m_sprite.setOrigin(25, 25);

	m_sprite.setPosition(m_position);
}

sf::FloatRect Zombie::getPosition()
{
	return m_sprite.getGlobalBounds();
}

sf::Sprite Zombie::getSprite()
{
	return m_sprite;
}

void Zombie::update(float elapsedTime, sf::Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	if (playerX > m_position.x)
	{
		m_position.x = m_position.x + m_speed * elapsedTime;
	}

	if (playerY > m_position.y)
	{
		m_position.y = m_position.y + m_speed * elapsedTime;
	}

	if (playerX < m_position.x)
	{
		m_position.x = m_position.x - m_speed * elapsedTime;
	}

	if (playerY < m_position.y)
	{
		m_position.y = m_position.y - m_speed * elapsedTime;
	}

	// move the sprite
	m_sprite.setPosition(m_position);

	// face sprite in the correct direction
	float angle = (atan2(playerY - m_position.y, playerX - m_position.x) * 180) / 3.141f;

	m_sprite.setRotation(angle);
}
