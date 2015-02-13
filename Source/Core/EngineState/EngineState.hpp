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
// File: EngineState.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines EngineState class.
// ========================================================================= //

#ifndef __ENGINESTATE_HPP__
#define __ENGINESTATE_HPP__

// ========================================================================= //

#include "Observer/Subject.hpp"
#include "stdafx.hpp"
#include "World/World.hpp"

// ========================================================================= //

class EngineState;

typedef std::shared_ptr<EngineState> EngineStatePtr;
typedef unsigned int EngineStateID;

// ========================================================================= //
// Abstract class which controls a single state to be run by the engine. Only 
// one state can be active at a time.
class EngineState
{
public:
    // Default initializes member variables.
    explicit EngineState(void);

    // Frees any data allocated by member variables.
    virtual ~EngineState(void) = 0;

    // Initializes state-specific data when it's entered.
    virtual void enter(void) = 0;

    // Cleans up any state-specific data on exit.
    virtual void exit(void) = 0;

    // Updates the state, should be called each frame when active.
    virtual void update(void) = 0;

    // Getters:

    // Returns Subject for adding Observer objects.
    Subject& getSubject(void);

    // Returns World for injecting dependencies in Engine.
    World& getWorld(void);

    // Setters:

    // Sets state to active or not. If true, the state will update itself.
    // Otherwise no updates will be performed.
    void setActive(const bool);

protected:
    // Subject for Engine's Observer.
    Subject m_subject;

    // State data.
    World m_world;
    bool m_active;
};

// ========================================================================= //

// Getters:

inline Subject& EngineState::getSubject(void){
    return m_subject;
}

inline World& EngineState::getWorld(void){
    return m_world;
}

// Setters:

inline void EngineState::setActive(const bool active){
    m_active = active;
}

// ========================================================================= //

#endif

// ========================================================================= //