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