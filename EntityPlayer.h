#pragma once

#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "Entity.h"

#include <glm/fwd.hpp>


class PlayerEntity : public Entity
{
private:
	float		m_health;

protected:
public:

	PlayerEntity( )
	{
		m_health = 100.0f;
	}
	~PlayerEntity()
	{
	}

	float			getHealth() const { return m_health; }
};

#endif // PLAYER_ENTITY_H

