
#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{
	for (auto i = this->m_attached_entities.begin(); i != this->m_attached_entities.end(); i++)
	{
		(*i)->m_attached_to = nullptr;
	}

	//TODO: remove self from attached (parent) vector too
}
