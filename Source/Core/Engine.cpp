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
#include "EngineState/IntroState.hpp"

// ========================================================================= //

Engine::Engine(void) :
m_root(nullptr),
m_renderWindow(nullptr),
m_viewport(nullptr),
m_log(nullptr),
m_timer(nullptr),
m_states(),
m_stateStack(),
m_active(false)
{
	m_states.reserve(5);
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

	// Allocate the main timer.
	m_timer.reset(new Ogre::Timer());

	// Register all needed game states.
	// @TODO: Define these in data.
	this->registerState(Engine::StateID::STATE_INTRO);

	return true;
}

// ========================================================================= //

void Engine::start(const EngineStateID id)
{
	m_active = true;

	// Push the specified state onto the active stack and run the game loop.
	this->pushState(id);

	while (m_active == true){
		// Check for window closing.
		if (m_renderWindow->isClosed()){
			m_active = false;
		}

		Ogre::WindowEventUtilities::messagePump();

		// Update the engine if the render window is active.
		if (m_renderWindow->isActive()){

			// Update the current state.
			m_stateStack.top()->update();

			// Render the updated frame.
			m_root->renderOneFrame();
		}
	}
}

// ========================================================================= //

void Engine::registerState(const EngineStateID id)
{
	std::shared_ptr<EngineState> state(nullptr);

	switch (id){
	default:
		return;

	case Engine::StateID::STATE_INTRO:
		state.reset(new IntroState());
		break;
	}

	Assert(state != nullptr, "Test");

	// Add dependencies and register in state list.
	state->injectDependencies(m_root, m_viewport);
	m_states.push_back(state);
}

// ========================================================================= //

void Engine::pushState(const EngineStateID id)
{
	Assert(id < Engine::StateID::NUM_STATES, "Invalid EngineStateID");

	// Get a pointer to the specified state.
	std::shared_ptr<EngineState> state = m_states[id];

	m_stateStack.push(state);

	// Initialize the state.	
	state->enter();
}

// ========================================================================= //

void Engine::popState(void)
{
	std::shared_ptr<EngineState> state = m_stateStack.top();
	state->exit();

	m_stateStack.pop();
	if (m_stateStack.empty() == true){
		m_active = false;
	}
}

// ========================================================================= //