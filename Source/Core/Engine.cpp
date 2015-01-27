// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Engine.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Engine class.
// ========================================================================= //

#include "Engine.hpp"

// ========================================================================= //

Engine::Engine(void) :
m_root(nullptr),
m_renderWindow(nullptr),
m_viewport(nullptr),
m_log(nullptr),
m_states()
{
	
}

// ========================================================================= //

Engine::~Engine(void)
{

}

// ========================================================================= //

bool Engine::init(void)
{
	// Create logging system.
	Ogre::LogManager* logMgr = new Ogre::LogManager();
	m_log = Ogre::LogManager::getSingleton().createLog("OgreLog.log");

	// Initialize Ogre's root component.
	m_root = new Ogre::Root();

	// @TODO: First check if this is the first run on the machine.
	// Set rendering device to default.
	Ogre::RenderSystemList renderers = m_root->getAvailableRenderers();
	m_root->setRenderSystem(*renderers.begin());

	// Prompt the user with config dialog.
	if (!m_root->showConfigDialog()){
		return false;
	}

	// Initialize render window.
	m_renderWindow = m_root->initialise(true, "Engine");

	// Initialize viewport for render window.
	m_viewport = m_renderWindow->addViewport(nullptr);
	m_viewport->setBackgroundColour(Ogre::ColourValue::Black);

	// Activate the render window.
	m_renderWindow->setActive(true);

	return true;
}

// ========================================================================= //