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

#include "ComponentDecls.hpp"
#include "stdafx.hpp"

// ========================================================================= //
// Holds data & behavior(if necessary) for specific Entity needs.
class Component
{
public:
    // Initializes m_name to "nil".
    explicit Component(void);

    // Empty destructor.
    virtual ~Component(void) = 0;

    // Interface function for initialization.
    virtual void init(void) { }

    // Interface function for destruction.
    virtual void destroy(void) { }

    // Interface function for updating.
    virtual void update(void) { }

    // Handles a message received from parent Entity.
    virtual void message(ComponentMessage& msg) { }

    // Triggered for all attached components when a new component is 
    // attached to an Entity.
    virtual void onComponentAttached(ComponentPtr component) { }

    // Triggered for all attached components when a component is 
    // detached from an Entity.
    virtual void onComponentDetached(ComponentPtr component) { }

    // Getters:

    // Returns pointer to world that created it.
    std::shared_ptr<World> getWorld(void) const;

    // Setters:

    // Sets pointer to world that created it.
    void setWorld(std::shared_ptr<World> world);

private:
    // Keep a pointer to the world that created it.
    std::shared_ptr<World> m_world;
};

// ========================================================================= //

// Getters:

inline std::shared_ptr<World> Component::getWorld(void) const{
    return m_world;
}

// Setters:

inline void Component::setWorld(std::shared_ptr<World> world){
    m_world = world;
}

// ========================================================================= //

#endif

// ========================================================================= //