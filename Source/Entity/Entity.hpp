// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Entity.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Entity class.
// ========================================================================= //

#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

// ========================================================================= //

#include "Component/Component.hpp"

// ========================================================================= //

typedef std::list<ComponentPtr> ComponentList;
typedef unsigned int EntityID;

// ========================================================================= //
// Abstract class for anything in the game world, such as players, boxes,
// walls, doors, particle systems, etc.
class Entity
{
public:
	// Default initializes member data.
	explicit Entity(void);

	// Empty desctructor.
	virtual ~Entity(void) = 0;

	// Registers component with the entity.
	void addComponent(ComponentPtr);

	// Getters:

	// Returns entity's ID.
	const EntityID getID(void) const;

	// Returns pointer to component of named type. Returns a nullptr if
	// it doesn't exist.
	ComponentPtr getComponentPtr(const std::string& name) const;

	// Setters:

	// Sets a new ID for the entity.
	void setID(const EntityID);

private:
	EntityID m_id;
	ComponentList m_components;
};

// ========================================================================= //

// Getters:

inline const EntityID Entity::getID(void) const{
	return m_id;
}

// Setters:

inline void Entity::setID(const EntityID id){
	m_id = id;
}

// ========================================================================= //

#endif

// ========================================================================= //