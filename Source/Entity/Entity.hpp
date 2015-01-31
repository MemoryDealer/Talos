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

#include "stdafx.hpp"

// ========================================================================= //

class Component;
struct ComponentMessage;
class Entity;
class World;

typedef Component* ComponentPtr;
typedef unsigned int EntityID;
typedef Entity* EntityPtr;
typedef std::list<ComponentPtr> ComponentList;

// ========================================================================= //
// Abstract class for anything in the game world, such as players, boxes,
// walls, doors, particle systems, etc.
class Entity
{
public:
	// Default initializes member data.
	explicit Entity(void);

	// Empty desctructor.
	~Entity(void);

	// Calls init() on all attached components.
	virtual void init(World& world);

	// Calls destroy() on all attached components.
	virtual void destroy(World& world);

	// Calls update() on all attached components.
	virtual void update(World& world);

	// Registers component with the entity.
	void attachComponent(ComponentPtr);

	// Unregisters component from the entity.
	void detachComponent(ComponentPtr);

	// Broadcasts ComponentMessage to all attached Components.
	void message(const ComponentMessage&);

	// Getters:

	// Returns entity's ID.
	const EntityID getID(void) const;

	// Returns pointer to component of named type. Returns a nullptr if
	// it doesn't exist.
	// @TODO: Add component-specific getter functions? This would avoid 
	// having to cast the ComponentPtr everytime it's retrieved.
	ComponentPtr getComponentPtr(const std::string& name) const;

	// Returns next EntityPtr as part of the EntityPool.
	EntityPtr getNext(void) const;

	// Setters:

	// Sets a new ID for the entity.
	void setID(const EntityID);

	// Sets the next EntityPtr for the EntityPool.
	void setNext(EntityPtr);

private:
	ComponentList m_components;

	// Save memory for the EntityPool (see EntityPool.cpp).
	union{
		EntityID m_id;
		EntityPtr m_next;
	};
};

// ========================================================================= //

// Getters:

inline const EntityID Entity::getID(void) const{
	return m_id;
}

inline EntityPtr Entity::getNext(void) const{
	return m_next;
}

// Setters:

inline void Entity::setID(const EntityID id){
	m_id = id;
}

inline void Entity::setNext(EntityPtr next){
	m_next = next;
}

// ========================================================================= //

#endif

// ========================================================================= //