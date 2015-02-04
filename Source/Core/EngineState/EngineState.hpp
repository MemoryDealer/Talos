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