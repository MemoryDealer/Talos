// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: EngineState.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements EngineState class.
// ========================================================================= //

#include "EngineState.hpp"

// ========================================================================= //

EngineState::EngineState(const EngineStateID id) :
m_scene(nullptr),
m_camera(nullptr),
m_id(0)
{

}

// ========================================================================= //

EngineState::~EngineState(void)
{

}

// ========================================================================= //

void EngineState::injectDependencies(Ogre::Root* root, 
	Ogre::Viewport* viewport)
{
	m_root = root;
	m_viewport = viewport;
}

// ========================================================================= //