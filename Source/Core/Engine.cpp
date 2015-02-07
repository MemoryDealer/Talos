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
#include "Input/Input.hpp"
#include "Physics/Physics.hpp"
#include "Resources.hpp"
#include "World/World.hpp"

// ========================================================================= //

Engine::Engine(void) :
m_root(nullptr),
m_renderWindow(nullptr),
m_viewport(nullptr),
m_log(nullptr),
m_timer(nullptr),
m_sdlWindow(nullptr),
m_ceguiRenderer(nullptr),
m_physics(nullptr),
m_input(nullptr),
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

	// Ogre3D and SDL:

	// Create logging system.
	Ogre::LogManager* logMgr = new Ogre::LogManager();
	m_log = Ogre::LogManager::getSingleton().createLog("OgreLog.log");

	// Initialize Ogre's root component.
	m_root = new Ogre::Root();

	// Set render system.
	Ogre::RenderSystem* rs = m_root->getRenderSystemByName("OpenGL Rendering Subsystem");
	if (!(rs->getName() == "OpenGL Rendering Subsystem")){
		return false;
	}
	m_root->setRenderSystem(rs);	

	// Initialize Ogre root with render system.
	m_root->initialise(false);

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
		return false;
	}

	// Create the main window with SDL.
	const int width = 1024;
	const int height = 768;
	m_sdlWindow = SDL_CreateWindow(
		"Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_RESIZABLE);

	// Bind mouse to window.
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(0);

	// Give the window handle to Ogre.
	SDL_SysWMinfo wmInfo;
	SDL_GetVersion(&wmInfo.version);
	if (SDL_GetWindowWMInfo(m_sdlWindow, &wmInfo) == SDL_FALSE){
		return false;
	}
	Ogre::String wHandle;
#ifdef WIN32
	wHandle = Ogre::StringConverter::toString(
		reinterpret_cast<unsigned long>(wmInfo.info.win.window));
#elif __LINUX__
	wHandle = Ogre::StringConverter::toString(
		reinterpret_cast<unsigned long>(wmInfo.info.x11.window));
#endif

	Ogre::NameValuePairList params;
	params["externalWindowHandle"] = wHandle;
	params["vsync"] = "true";
	m_renderWindow = m_root->createRenderWindow("Engine",
												width,
												height,
												false,
												&params);

	// Initialize Ogre viewport for render window.
	m_viewport = m_renderWindow->addViewport(nullptr);
	m_viewport->setBackgroundColour(Ogre::ColourValue::Black);

	// Activate the  Ogre render window.
	m_renderWindow->setActive(true);

	// Load resources for Ogre (from Resources.hpp).
	loadOgreResources();

	// Allocate the main Ogre timer.
	m_timer.reset(new Ogre::Timer());
	m_timer->reset(); // Activate timer.

	// === //

	// CEGUI:

	// Bootstrap the rendering system.
	m_ceguiRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*(m_root->getRenderTarget("Engine")));

	// Setup default CEGUI skins.
	// @TODO: Load from config file.
	CEGUI::SchemeManager::getSingleton().createFromFile(
		"AlfiskoSkin.scheme");
	/*CEGUI::SchemeManager::getSingleton().createFromFile(
		"GameMenu.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile(
		"Generic.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile(
		"VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile(
		"TaharezLook.scheme");*/
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
	CEGUI::System::getSingleton().getDefaultGUIContext().
		setDefaultFont("DejaVuSans-10");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().
		setDefaultImage("AlfiskoSkin/MouseArrow");

	// === //

	// PhysX:

	m_physics.reset(new Physics());
	if (m_physics->init() == false){
		return false;
	}

	// === //

	// Engine:

	// Allocate input handler.
	m_input.reset(new Input());

	// Register all needed game states.
	// @TODO: Define these in data.
	this->registerState(Engine::StateID::STATE_INTRO);

	// === //

	return true;
}

// ========================================================================= //

void Engine::shutdown(void)
{
	m_physics->destroy();
	delete m_root;
	delete Ogre::LogManager::getSingletonPtr();
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
			while (lag >= MS_PER_UPDATE && m_active == true){
				m_stateStack.top()->update();

				lag -= MS_PER_UPDATE;
			}

			// Update CEGUI.
			CEGUI::System::getSingleton().
				injectTimePulse(static_cast<float>(lag / MS_PER_UPDATE));

			// Render the updated frame.
			m_root->renderOneFrame(Ogre::Real(lag / MS_PER_UPDATE));
		}
	}
}

// ========================================================================= //

void Engine::registerState(const EngineStateID id)
{
	EngineStatePtr state(nullptr);

	switch (id){
	default:
		return;

	case Engine::StateID::STATE_INTRO:
		state.reset(new IntroState());
		break;
	}

	Assert(state != nullptr, "Test");

	// Add dependencies.
	World::Dependencies deps;
	deps.root = m_root;
	deps.viewport = m_viewport;
	deps.physics = m_physics;
	deps.input = m_input.get();
	state->getWorld().injectDependencies(deps);

	// Add self as an Observer to listen for events.
	state->getSubject().addObserver(this);

	// Add state to list of states.
	m_states.push_back(state);
}

// ========================================================================= //

void Engine::pushState(const EngineStateID id)
{
	Assert(id < Engine::StateID::NUM_STATES, "Invalid EngineStateID");

	// Get a pointer to the specified state.
	EngineStatePtr state = m_states[id];

	m_stateStack.push(state);

	// Initialize the state.	
	state->setActive(true);
	state->enter();
}

// ========================================================================= //

void Engine::popState(void)
{
	EngineStatePtr state = m_stateStack.top();
	state->setActive(false);
	state->exit();

	m_stateStack.pop();
	if (m_stateStack.empty() == true){
		m_active = false;
	}
}

// ========================================================================= //

void Engine::onNotify(const unsigned int id)
{
	switch (id){
	default:
		break;

	case Engine::Notification::POP:
		this->popState();
		break;
	}
}

// ========================================================================= //