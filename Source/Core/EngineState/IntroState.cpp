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

#include "Component/ComponentMessage.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/FirstPersonComponent.hpp"
#include "Component/InputComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/Player.hpp"
#include "IntroState.hpp"
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
	m_active = true;

	// Create scene manager.
	// @TODO: Move viewport injection to Engine.
	m_world.init();

	Ogre::SceneManager* scene = m_world.getSceneManager();

	scene->setAmbientLight(Ogre::ColourValue::White);

	scene->setSkyDome(true, "Clouds");

	m_player = m_world.createEntity();
	FirstPersonComponentPtr fpComponent = m_world.createFirstPersonComponent();
	m_player->attachComponent(fpComponent);
	CameraComponentPtr cameraComponent = m_world.createCameraComponent();
	m_player->attachComponent(cameraComponent);
	m_player->attachComponent(m_world.createInputComponent());
	m_player->init(m_world);

	fpComponent->attachCamera(cameraComponent->getCamera());

	m_ogre = m_world.createEntity();
	SceneComponentPtr sceneComponent = m_world.createSceneComponent();
	m_ogre->attachComponent(sceneComponent);
	ModelComponent* model = m_world.createModelComponent();
	model->setMeshFilename("ogrehead.mesh");
	m_ogre->attachComponent(model);
	m_ogre->init(m_world);

	sceneComponent->attachObject(model->getOgreEntity());
	sceneComponent->getSceneNode()->setPosition(0.0f, -10.0f, -50.0f);

	// Setup GUI.
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* root = wmgr.createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root);

	CEGUI::FrameWindow* fwnd = static_cast<CEGUI::FrameWindow*>(
		wmgr.createWindow("AlfiskoSkin/FrameWindow", "testWindow"));
	root->addChild(fwnd);
	fwnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f, 0.0f),
		CEGUI::UDim(0.25f, 0.0f)));
	fwnd->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
	fwnd->setText("Hello world!");
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
	m_world.update();
	static_cast<SceneComponentPtr>(m_ogre->getComponentPtr("SceneComponent"))
		->getSceneNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(1.0));
}

// ========================================================================= //