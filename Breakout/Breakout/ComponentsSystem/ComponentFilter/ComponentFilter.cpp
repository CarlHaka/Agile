#include "ComponentFilter.h"

bool ComponentFilter::DoesFilterPass(const std::vector<IComponent*> _components) const
{
	// Get the number of required/excluded components
	size_t noRequiredComponents = m_requiredComponents.size();
	size_t noExcludedComponents = m_excludedComponents.size();

	// If both are 0 there is no need to check
	if (noRequiredComponents == 0 && noExcludedComponents == 0)
		return true;

	/*
	Go through all required components
	If _components & requiredComponents match (same ID), decrease noRequiredComponents by 1
	When done, noRequiredComponents should be 0 (if a match was found for each required component)
	*/
	for (int i = 0; i < m_requiredComponents.size(); ++i)
	{
		for (int j = 0; j < _components.size(); ++j)
		{
			if (m_requiredComponents[i] == _components[j]->m_ID)
			{
				noRequiredComponents--;
				break;
			}
		}
	}

	// norequiredComponents is still > 0, not all required components exist
	if (noRequiredComponents > 0)
		return false;

	/*
	Go through all excluded components
	If a match between _components and excludedComponents is found return false
	*/
	for (int i = 0; i < m_excludedComponents.size(); ++i)
	{
		for (int j = 0; j < _components.size(); ++j)
		{
			if (m_excludedComponents[i] == _components[j]->m_ID)
				return false;
		}
	}

	return true;
}

void ComponentFilter::Clear()
{
	m_requiredComponents.clear();
	m_excludedComponents.clear();
}

