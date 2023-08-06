#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type)
{
	m_type = type;

	if (m_type == 1)
	{
		m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/health_pickup.png"));

		m_value = HEALTH_START;
	}
	else
	{
		m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/ammo_pickup.png"));

		m_value = AMMO_START;
	}

	m_sprite.setOrigin(25, 25);

	m_secondsToLive = START_SECONDS_TO_LIVE;
	m_secondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(sf::IntRect arena)
{
	m_arena.left = arena.left + 50;
	m_arena.width = arena.width - 50;
	m_arena.top = arena.top + 50;
	m_arena.height = arena.height - 50;

	spawn();
}

void Pickup::spawn()
{
	srand((int)time(0) / m_type);
	int x = (rand() % m_arena.width);
	srand((int)time(0) * m_type);
	int y = (rand() % m_arena.height);

	m_secondsSinceSpawn = 0;
	m_spawned = true;

	m_sprite.setPosition(x, y);
}

sf::FloatRect Pickup::getPosition()
{
	return m_sprite.getGlobalBounds();
}


sf::Sprite Pickup::getSprite()
{
	return m_sprite;
}

void Pickup::update(float elapsedTime)
{
	if (m_spawned)
		m_secondsSinceSpawn += elapsedTime;
	else
		m_secondsSinceDespawn += elapsedTime;

	if (m_secondsSinceSpawn > m_secondsToLive && m_spawned)
	{
		m_spawned = false;
		m_secondsSinceDespawn = 0;
	}

	if (m_secondsSinceDespawn > m_secondsToWait && !m_spawned)
		spawn();

}

bool Pickup::isSpawned()
{
	return m_spawned;
}

int Pickup::gotIt()
{
	m_spawned = false;
	m_secondsSinceDespawn = 0;
	return m_value;
}

void Pickup::upgrade()
{
	if (m_type == 1)
		m_value += (HEALTH_START * .5);
	else
		m_value += (AMMO_START * .5);

	m_secondsToLive += (START_SECONDS_TO_LIVE / 10);
	m_secondsToWait -= (START_WAIT_TIME / 10);
}
