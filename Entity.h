#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <vector>


class Entity
{
public:

	//void (*)(void)  onCreate;
	//void (*)(void)  onDestroy;
	//void (*)(void)	onAttach;
	//void (*)(void)	onDeattach;

	glm::vec3	getPosition() const { return m_position; }
	glm::vec3	getOrientation() const { return m_orientation; }
	float		getScale() const { return m_scale; }

	void		setPosition(const glm::vec3 &position) { m_position = position; }
	void		setOrientation(const glm::vec3 &orientation) { m_orientation = orientation; }
	void		setScale(const float &scale) { m_scale = scale; }

	void		attachToEntity(Entity& entity);
	void		deattach();

	Entity();
	~Entity();

protected:

	glm::vec3	m_position;
	glm::vec3	m_orientation;
	float		m_scale;
	Entity*		m_attached_to;
	std::vector<Entity*>	m_attached_entities;

private:
	//virtual ~Entity();
};
