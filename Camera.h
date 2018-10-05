#pragma once

#include "Entity.h"


class Camera : public Entity
{
public:
    Entity*		attached;
	glm::vec3	up_dir;
	float		field_of_view;
	float       zoom;
};
