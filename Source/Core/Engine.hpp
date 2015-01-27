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

#include <stdafx.hpp>

// ========================================================================= //

class EngineState{};

// ========================================================================= //
// Holds rendering components, manages engine state stack.
class Engine
{
public:
	
	Engine(void);

	// Frees 
	~Engine(void);

	// Sets up initial rendering components and stack for engine states.
	bool init(void);

private:
	// Ogre3D components.
	Ogre::Root*			m_root;
	Ogre::RenderWindow* m_renderWindow;
	Ogre::Viewport*		m_viewport;
	Ogre::Log*			m_log;

	// State management.
	std::stack<EngineState>	m_states;
};

// ========================================================================= //

#endif

// ========================================================================= //