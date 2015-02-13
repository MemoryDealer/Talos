// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
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
#include "Component/LightComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/Entity.hpp"
#include "Input/Input.hpp"
#include "IntroState.hpp"
#include "Physics/PScene.hpp"
#include "Rendering/DynamicLines.hpp"
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
	m_world.getPScene()->loadDebugDrawer();

	// Setup visual scene settings.
	m_world.setLightColour(0.25f, 0.15f, 0.20f);
	m_world.setFog(Ogre::FOG_EXP2, 0.f, 0.f, 0.f, 0.05f);

	// Create scene manager.
	Ogre::SceneManager* scene = m_world.getSceneManager();

	scene->setSkyDome(true, "Clouds");

	// Create player entity.
	m_player = m_world.createEntity();
	ActorComponentPtr actorComponent = m_world.createActorComponent();
	actorComponent->init(m_player, m_world);
	m_player->attachComponent(actorComponent);
	CameraComponentPtr cameraComponent = m_world.createCameraComponent();
	cameraComponent->init(m_player, m_world);
	m_player->attachComponent(cameraComponent);

	// Wire up camera component to actor component.
	actorComponent->attachCamera(cameraComponent);

	// Create a dynamic object as an ogre mesh.
	m_ogre = m_world.createEntity();
	SceneComponentPtr sceneComponent = m_world.createSceneComponent();
	sceneComponent->init(m_ogre, m_world);
	m_ogre->attachComponent(sceneComponent);
	ModelComponent* model = m_world.createModelComponent();
	model->init(m_world, "ogrehead.mesh");
	m_ogre->attachComponent(model);
	PhysicsComponentPtr physicsC = m_world.createPhysicsComponent();
	physicsC->init(m_world, m_ogre, PhysicsComponent::Type::DYNAMIC, 
				  // PxBoxGeometry(1.f, 1.f, 1.f), 
				  PxSphereGeometry(5.5f),
				   0.5f, 0.5f, 0.1f
				   );
	physicsC->translate(25.f, 0.f, 0.f);
	physicsC->getDynamicActor()->setLinearVelocity(PxVec3(1.5f, 0.f, 0.f));
	m_ogre->attachComponent(physicsC);
	LightComponentPtr lightC = m_world.createLightComponent();
	lightC->init(m_world, LightComponent::Type::POINT);
	lightC->setColour(50.f, 0.f, 50.f);
	lightC->setRange(175.f);
	m_ogre->attachComponent(lightC);

	//sceneComponent->attachModel(model);
	sceneComponent->attachLight(lightC);

	// Plane.
	EntityPtr board = m_world.createEntity();
	SceneComponentPtr sceneC = m_world.createSceneComponent();
	sceneC->init(board, m_world);
	board->attachComponent(sceneC);
	ModelComponentPtr modelC = m_world.createModelComponent();
	modelC->init(m_world, "Plane/Board", "Board");
	board->attachComponent(modelC);
	physicsC = m_world.createPhysicsComponent();
	physicsC->init(m_world, board, PhysicsComponent::Type::STATIC, physx::PxBoxGeometry(75.f, 5.f, 75.f));
	//physicsC->setOrientation(0.9238795325112867f, -0.3826834323650897f, 0.f, 0.f);
	physicsC->translate(0.f, -50.f, 0.f);
	//physicsC->rotate(45.f, 0.f, 0.f);
	board->attachComponent(physicsC);

	sceneC->attachModel(modelC);

	// Setup GUI.
	/*CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window* root = wmgr.createWindow("DefaultWindow", "root");
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

	if (m_world.checkEntities() == false){
		throw std::exception("World::checkEntities() reported uninitialized Entity");
	}
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
					// Send input commands to the player.
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