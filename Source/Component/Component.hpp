// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Component.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Component class.
// ========================================================================= //

#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

// Forward declare all components here to avoid repeating elsewhere.
class Component;
class ActorComponent;
class CameraComponent;
class LightComponent;
class ModelComponent;
class PhysicsComponent;
class SceneComponent;
// Other forward declarations.
struct ComponentMessage;
class Entity;
class World;

typedef Component* ComponentPtr;
typedef ActorComponent* ActorComponentPtr;
typedef CameraComponent* CameraComponentPtr;
typedef LightComponent* LightComponentPtr;
typedef ModelComponent* ModelComponentPtr;
typedef PhysicsComponent* PhysicsComponentPtr;
typedef SceneComponent* SceneComponentPtr;
typedef Entity* EntityPtr;

// ========================================================================= //
// Holds data & behavior(if necessary) for specific Entity needs.
class Component
{
public:
	enum Type{
		Nil = 0,
		Actor,
		Camera,
		Light,
		Model,
		Physics,
		Scene
	};

	// Initializes m_name to "nil".
	explicit Component(void);

	// Empty destructor.
	virtual ~Component(void) = 0;

	// Interface function for initialization.
	virtual void init(EntityPtr, World&) = 0;

	// Interface function for destruction.
	virtual void destroy(EntityPtr, World&) = 0;

	// Interface function for updating.
	virtual void update(EntityPtr, World&) = 0;

	// Handles a message received from parent Entity.
	virtual void message(const ComponentMessage&) = 0;

	// Getters:

	// Returns the name of the component type.
	const Type getType(void) const;

	// Returns true if Component has been initialized.
	const bool isInitialized(void) const;

	// Setters:

	// Sets initialization state of Component.
	void setInitialized(const bool);

protected:

	// Sets type of Component, called by derived Component constructor.
	void setType(const Type);

private:
	Type m_type;
	bool m_initialized;
};

// ========================================================================= //

// Getters:

inline const Component::Type Component::getType(void) const{
	return m_type;
}

inline const bool Component::isInitialized(void) const{
	return m_initialized;
}

// Setters:

inline void Component::setType(const Type type){
	m_type = type;
}

inline void Component::setInitialized(const bool initialized){
	m_initialized = initialized;
}

// ========================================================================= //

#endif

// ========================================================================= //