#include "Player.h"

Player::Player()
{
	m_speed = static_cast<float>(START_SPEED);
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;
	// Associate a texture with the sprite
	// !!Watch this space!!
	m_texture.loadFromFile("graphics/player.png");
	m_sprite.setTexture(m_texture);
	// Set the origin of the sprite to the center, 
	// for smooth rotation
	m_sprite.setOrigin(25, 25);
}

void Player::spawn(sf::IntRect arena,
	sf::Vector2f resolution,
	int tileSize)
{
	// Place the player in the middle of the arena
	m_position.x = (float)arena.width / 2.f;
	m_position.y = (float)arena.height / 2.f;
	// Copy the details of the arena 
	// to the player's m_Arena
	m_arena.left = arena.left;
	m_arena.width = arena.width;
	m_arena.top = arena.top;
	m_arena.height = arena.height;
	// Remember how big the tiles are in this arena
	m_tileSize = tileSize;
	// Store the resolution for future use
	m_resolution.x = resolution.x;
	m_resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
	m_speed = static_cast<float>(START_SPEED);
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;
}

sf::Time Player::getLastHitTime()
{
	return m_lastHit;
}
bool Player::hit(sf::Time timeHit)
{
	if (timeHit.asMilliseconds() - m_lastHit.asMilliseconds() > 200)
	{
		m_lastHit = timeHit;
		m_health -= 10;
		return true;
	}
	else
	{
		return false;
	}
}

sf::FloatRect Player::getPosition()
{
	return m_sprite.getGlobalBounds();
}

sf::Vector2f Player::getCenter()
{
	return m_position;
}

float Player::getRotation()
{
	return m_sprite.getRotation();
}

sf::Sprite Player::getSprite()
{
	return m_sprite;
}

int Player::getHealth()
{
	return m_health;
}

void Player::moveLeft()
{
	m_leftPressed = true;
}
void Player::moveRight()
{
	m_rightPressed = true;
}
void Player::moveUp()
{
	m_upPressed = true;
}
void Player::moveDown()
{
	m_downPressed = true;
}

void Player::stopLeft()
{
	m_leftPressed = false;
}
void Player::stopRight()
{
	m_rightPressed = false;
}
void Player::stopUp()
{
	m_upPressed = false;
}
void Player::stopDown()
{
	m_downPressed = false;
}

void Player::update(float elapsedTime, sf::Vector2i mousePosition)
{
	if (m_upPressed)
	{
		m_position.y -= m_speed * elapsedTime;
	}

	if (m_downPressed)
	{
		m_position.y += m_speed * elapsedTime;
	}

	if (m_rightPressed)
	{
		m_position.x += m_speed * elapsedTime;
	}

	if (m_leftPressed)
	{
		m_position.x -= m_speed * elapsedTime;
	}

	m_sprite.setPosition(m_position);

	// keep the player in the arena
	if (m_position.x > (float)m_arena.width - (float)m_tileSize)
	{
		m_position.x = (float)m_arena.width - (float)m_tileSize;
	}

	if (m_position.x < (float)m_arena.left + (float)m_tileSize)
	{
		m_position.x = (float)m_arena.left + (float)m_tileSize;
	}

	if (m_position.y > (float)m_arena.height - (float)m_tileSize)
	{
		m_position.y = (float)m_arena.height - (float)m_tileSize;
	}

	if (m_position.y < (float)m_arena.top + (float)m_tileSize)
	{
		m_position.y = (float)m_arena.top + (float)m_tileSize;
	}

	// calculate the angle the player is facing
	float angle = (atan2((float)mousePosition.y - m_resolution.y / 2.f, (float)mousePosition.x - m_resolution.x / 2.f) * 180.f) / 3.141f;
	
	m_sprite.setRotation(angle);
	
}

void Player::upgradeSpeed()
{
	// 20% upgrade
	m_speed += (int)(START_SPEED * .2f);
}

void Player::upgradeHealth()
{
	// 20% upgrade
	m_maxHealth += (int)(START_HEALTH * .2f);
}

void Player::increaseHealthLevel(int amount)
{
	m_health += amount;
	
	// but not beyond he maximum
	if (m_health > m_maxHealth)
	{
		m_health = m_maxHealth;
	}
}