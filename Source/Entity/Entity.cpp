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

#include "Component/Component.hpp"
#include "Component/ComponentMessage.hpp"
#include "Entity.hpp"

// ========================================================================= //

Entity::Entity(void) :
m_components(),
m_id(0),
m_next(nullptr)
{

}

// ========================================================================= //

Entity::~Entity(void)
{

}

// ========================================================================= //

void Entity::init(World& world)
{
	for (ComponentList::iterator itr = m_components.begin();
		 itr != m_components.end();
		 ++itr){
		(*itr)->init(this, world);
	}
}

// ========================================================================= //

void Entity::destroy(World& world)
{
	for (ComponentList::iterator itr = m_components.begin();
		 itr != m_components.end();
		 ++itr){
		(*itr)->destroy(this, world);
	}
}

// ========================================================================= //

void Entity::update(World& world)
{
	for (ComponentList::iterator itr = m_components.begin();
		 itr != m_components.end();
		 ++itr){
		(*itr)->update(this, world);
	}
}

// ========================================================================= //

void Entity::attachComponent(ComponentPtr component)
{
	m_components.push_back(component);
}

// ========================================================================= //

void Entity::detachComponent(ComponentPtr component)
{

}

// ========================================================================= //

void Entity::message(const ComponentMessage& msg)
{
	if (msg.type == ComponentMessageType::NIL){
		return;
	}

	// Broadcast to all attached components.
	for (ComponentList::iterator itr = m_components.begin();
		 itr != m_components.end();
		 ++itr){
		(*itr)->message(msg);
	}
}

// ========================================================================= //

ComponentPtr Entity::getComponentPtr(const std::string& name) const
{
	// @TODO: WHY does this iterator need to be const?
	for (ComponentList::const_iterator itr = m_components.begin();
		 itr != m_components.end();
		 ++itr){
		// Determine if Component's name matches. This is considerably more
		// efficient than run-time dynamic type checking.
		if ((*itr)->getName() == name){
			return *itr;
		}
	}

	return nullptr;
}

// ========================================================================= //