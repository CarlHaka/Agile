#include "Entity.h"


Entity::Entity()
: m_state(DEAD)
{
}

Entity::Entity(int _id)
: m_id(_id), m_state(DEAD)
{
}

Entity::~Entity()
{
	RemoveAllComponents();
}

void Entity::AddComponent(IComponent* base, TypeID _componentTypeId)
{
	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
		{
			// Log Warning
			return;
		}
	}

	base->m_ID = _componentTypeId;
	m_components.push_back(base);
}

void Entity::RemoveComponent(int _componentTypeId)
{

	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
		{
			m_components.erase(m_components.begin() + i);
			break;
		}
	}

}

IComponent* Entity::GetComponent(int _componentTypeId)
{

	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
			return m_components[i];
	}

	return 0;

}

bool Entity::HasComponent(int _componentTypeId)
{

	for (int i = 0; i < m_components.size(); ++i)
	{
		if (_componentTypeId == m_components[i]->m_ID)
			return true;
	}

	return false;
}

bool Entity::RemoveAllComponents()
{
	for (int i = m_components.size() - 1; i >= 0; --i)
	{
		IComponent* iComponent = m_components[i];
		delete iComponent;

		m_components.erase(m_components.begin() + i);
	}

	return true;
}

std::vector<IComponent*> Entity::GetComponents()
{
	return m_components;
}

int Entity::GetId(void)
{
	return m_id;
}