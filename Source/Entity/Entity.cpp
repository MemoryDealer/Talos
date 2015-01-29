// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Entity.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Entity class.
// ========================================================================= //

#include "Entity.hpp"

// ========================================================================= //

Entity::Entity(void) :
m_id(0),
m_components()
{

}

// ========================================================================= //

Entity::~Entity(void)
{

}

// ========================================================================= //

void Entity::addComponent(ComponentPtr component)
{
	m_components.push_back(component);
}

// ========================================================================= //

ComponentPtr Entity::getComponentPtr(const std::string& name) const
{
	/*for (ComponentList::iterator itr = m_components.begin();
		 itr != m_components.end();
		 ++itr){
		if (itr->get()->getName() == name){
			return *itr;
		}
	}*/

	return nullptr;
}

// ========================================================================= //