// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: EngineState.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines EngineState class.
// ========================================================================= //

#ifndef __ENGINESTATE_HPP__
#define __ENGINESTATE_HPP__

// ========================================================================= //

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

	World& getWorld(void) const;

protected:
	// Ogre3D components.
	Ogre::Root*	m_root; // Each engine state will have m_root injected.
	Ogre::Viewport* m_viewport; // Same as m_root.

	// State data.
	World m_world;
	bool m_active;
};

// ========================================================================= //

// Getters:

inline World& EngineState::getWorld(void) const{
	return const_cast<World&>(m_world);
}

// ========================================================================= //

#endif

// ========================================================================= //