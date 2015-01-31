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
#include "Resources.hpp"

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
	// === //

	// Ogre3D:

	// Create logging system.
	Ogre::LogManager* logMgr = new Ogre::LogManager();
	m_log = Ogre::LogManager::getSingleton().createLog("OgreLog.log");

	// Initialize Ogre's root component.
	m_root = new Ogre::Root();

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

	// Load resources for Ogre (from Resources.hpp).
	loadOgreResources();

	// Allocate the main timer.
	m_timer.reset(new Ogre::Timer());
	m_timer->reset(); // Activate timer.

	// === //

	// SDL:

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
		return false;
	}

	// Associate the Ogre3D window with the SDL_Window.
	unsigned long data = 0;
	m_renderWindow->getCustomAttribute("WINDOW", &data);
	SDL_Window* w = SDL_CreateWindowFrom(reinterpret_cast<void*>(data));

	// === //

	// Engine:

	// Register all needed game states.
	// @TODO: Define these in data.
	this->registerState(Engine::StateID::STATE_INTRO);

	// === //

	return true;
}

// ========================================================================= //

void Engine::start(const EngineStateID id)
{
	m_active = true;

	// Push the specified state onto the active stack and run the game loop.
	this->pushState(id);

	const double MS_PER_UPDATE = 16.0;
	double prev = m_timer->getMilliseconds();
	double lag = 0.0;

	while (m_active == true){
		// Check for window closing.
		if (m_renderWindow->isClosed()){
			m_active = false;
		}

		Ogre::WindowEventUtilities::messagePump();

		// Update the engine if the render window is active.
		if (m_renderWindow->isActive()){

			double current = m_timer->getMilliseconds();
			double elapsed = current - prev;
			prev = current;
			lag += elapsed;

			// Update the current state.
			while (lag >= MS_PER_UPDATE){
				m_stateStack.top()->update();

				lag -= MS_PER_UPDATE;
			}

			// Render the updated frame.
			m_root->renderOneFrame(Ogre::Real(lag / MS_PER_UPDATE));
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