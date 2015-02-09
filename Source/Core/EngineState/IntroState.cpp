// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: IntroState.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements IntroState class.
// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/ActorComponent.hpp"
#include "Component/ComponentMessage.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Input/Input.hpp"
#include "IntroState.hpp"
#include "Physics/PScene.hpp"
#include "World/World.hpp"

// ========================================================================= //

IntroState::IntroState(void) :
EngineState()
{

}

// ========================================================================= //

IntroState::~IntroState(void)
{

}

// ========================================================================= //

void IntroState::enter(void)
{
	m_world.init();
	m_world.getInput()->setMode(Input::Mode::PLAYER);

	// Create scene manager.
	Ogre::SceneManager* scene = m_world.getSceneManager();

	scene->setAmbientLight(Ogre::ColourValue::White);

	scene->setSkyDome(true, "Clouds");

	// Create player entity.
	m_player = m_world.createEntity();
	ActorComponentPtr actorComponent = m_world.createActorComponent();
	m_player->attachComponent(actorComponent);
	CameraComponentPtr cameraComponent = m_world.createCameraComponent();
	m_player->attachComponent(cameraComponent);
	m_player->init(m_world);

	// Wire up camera component to actor component.
	actorComponent->attachCamera(cameraComponent);

	// Create a dynamic object as an ogre mesh.
	m_ogre = m_world.createEntity();
	SceneComponentPtr sceneComponent = m_world.createSceneComponent();
	m_ogre->attachComponent(sceneComponent);
	ModelComponent* model = m_world.createModelComponent();
	model->init(m_world, "ogrehead.mesh");
	m_ogre->attachComponent(model);
	PhysicsComponentPtr physicsC = m_world.createPhysicsComponent();
	physicsC->init(m_world, PhysicsComponent::Type::DYNAMIC, PxSphereGeometry(10.f));
	physicsC->translate(0.f, 150.f, 0.f);
	m_ogre->attachComponent(physicsC);
	m_ogre->init(m_world);

	sceneComponent->attachModel(model);
	sceneComponent->getSceneNode()->setPosition(0.f, -10.f, -50.f);

	// Setup GUI.
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	/*CEGUI::Window* root = wmgr.createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root);

	CEGUI::FrameWindow* fwnd = static_cast<CEGUI::FrameWindow*>(
		wmgr.createWindow("AlfiskoSkin/FrameWindow", "testWindow"));
	root->addChild(fwnd);
	fwnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f, 0.f),
		CEGUI::UDim(0.25f, 0.f)));
	fwnd->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0.f), CEGUI::UDim(0.5f, 0.f)));
	fwnd->setText("Hello world!");

	CEGUI::Window* quit = wmgr.createWindow("AlfiskoSkin/Button", "root/quit");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0.f), CEGUI::UDim(0.05f, 0.f)));
	fwnd->addChild(quit);*/
	//quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::quit, this));

	// Plane.
	EntityPtr board = m_world.createEntity();
	SceneComponentPtr sceneC = m_world.createSceneComponent();
	board->attachComponent(sceneC);
	ModelComponentPtr modelC = m_world.createModelComponent();
	modelC->init(m_world, "Plane/Board", "Board");
	board->attachComponent(modelC);
	physicsC = m_world.createPhysicsComponent();
	physicsC->init(m_world, PhysicsComponent::Type::STATIC, physx::PxBoxGeometry(750.f, 0.5f, 750.f));
	physicsC->translate(0.f, -50.f, 0.f);
	board->attachComponent(physicsC);
	board->init(m_world);

	sceneC->attachModel(modelC);

	/*PxRigidStatic* p = PxCreatePlane(*m_world.getPScene()->m_physx,
									 PxPlane(PxVec3(0.f, 1.f, 0.f), 50.f),
									 *mat);*/
}

// ========================================================================= //

void IntroState::exit(void)
{
	m_player->destroy(m_world);
	m_world.destroy();
}

// ========================================================================= //

void IntroState::update(void)
{
	if (m_active == true){
		m_world.update();
		m_world.getPScene()->simulate();

		// Poll SDL for events.
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			switch (e.type){
			default:
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_KEYDOWN:
				{
					CommandPtr command = m_world.getInput()->handle(e);
					if (command != nullptr){
						command->execute(m_player);
					}
				}
				break;

			case SDL_KEYUP:
				{
					CommandPtr command = m_world.getInput()->handle(e);
					if (command != nullptr){
						command->unexecute(m_player);
					}
				}
				break;

			case SDL_QUIT:
				m_subject.notify(1);
				break;
			}
		}
	}
}

// ========================================================================= //