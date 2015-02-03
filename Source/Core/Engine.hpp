// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Engine.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Engine class.
// ========================================================================= //

#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

// ========================================================================= //

#include "stdafx.hpp"
#include "EngineState/EngineState.hpp"

// ========================================================================= //

typedef std::stack<EngineStatePtr> EngineStateStack;
typedef std::vector<EngineStatePtr> EngineStateList;

// ========================================================================= //
// Holds rendering components, manages engine state stack.
class Engine
{
public:
	// Reserves contiguous memory for EngineState's.
	explicit Engine(void);

	// Empty destructor.
	~Engine(void);

	// Sets up initial rendering components and stack for engine states.
	bool init(void);

	// Updates the active state and calculates elapsed time. This is the main
	// game update loop.
	void start(const EngineStateID);

	// Allocates an EngineState object, adds it to the list of states, and 
	// injects dependencies.
	void registerState(const EngineStateID);

	// Pushes a new active state onto the active state stack.
	void pushState(const EngineStateID);

	// Pops the active state off the active state stack, activating the next one.
	// If the stack is empty, the engine is shut down.
	void popState(void);

	// Enumerations:

	enum StateID{
		STATE_INTRO = 0,
		STATE_MAIN_MENU,
		STATE_GAME,
		STATE_PAUSED,

		NUM_STATES
	};

private:
	// Ogre3D components.
	Ogre::Root*	m_root;
	Ogre::RenderWindow* m_renderWindow;
	Ogre::Viewport*	m_viewport;
	Ogre::Log* m_log;
	std::shared_ptr<Ogre::Timer> m_timer; // The core engine timer.

	// SDL components.
	SDL_Window* m_sdlWindow;

	// CEGUI components.
	CEGUI::OgreRenderer* m_ceguiRenderer;

	// State management.
	EngineStateList m_states;
	EngineStateStack m_stateStack;

	// Engine data.
	bool m_active;
};

// ========================================================================= //

#endif

// ========================================================================= //