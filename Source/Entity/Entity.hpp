// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
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

class ActorComponent;
class Component;
struct ComponentMessage;
class Entity;
class World;

typedef ActorComponent* ActorComponentPtr;
typedef Component* ComponentPtr;
typedef unsigned int EntityID;
typedef Entity* EntityPtr;
typedef std::list<ComponentPtr> ComponentList;

// ========================================================================= //
// Class for anything in the game world, such as players, boxes,
// walls, doors, particle systems, etc. Components can be attached and 
// removed at run-time.
class Entity final
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

	// Registers Component with the entity. Returns a pointer to the newly
	// attached Component for convenience.
	ComponentPtr attachComponent(const ComponentPtr);

	// Unregisters component from the entity.
	void detachComponent(const ComponentPtr);

	// Checks if all attached Components have been initialized. Returns true
	// if so.
	const bool checkComponents(void) const;

	// Broadcasts ComponentMessage to all attached Components.
	void message(const ComponentMessage&);

	// Getters:

	// Returns entity's ID.
	const EntityID getID(void) const;

	// Returns pointer to component of named type. Returns a nullptr if
	// it doesn't exist.
	// @TODO: Add component-specific getter functions? This would avoid 
	// having to cast the ComponentPtr everytime it's retrieved.
	ComponentPtr getComponentPtr(const unsigned int) const;

	ActorComponentPtr getActorComponent(void) const;

	// Returns next EntityPtr as part of the EntityPool.
	EntityPtr getNext(void) const;

	// Setters:

	// Sets a new ID for the entity.
	void setID(const EntityID);

	// Sets the next EntityPtr for the EntityPool.
	void setNext(const EntityPtr);

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

inline void Entity::setNext(const EntityPtr next){
	m_next = next;
}

// ========================================================================= //

#endif

// ========================================================================= //