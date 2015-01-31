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
	m_world.init(m_root, m_viewport);

	Ogre::SceneManager* scene = m_world.getSceneManager();

	scene->setAmbientLight(Ogre::ColourValue::White);

	scene->setSkyDome(true, "Clouds");

	m_player = m_world.createEntity();
	FirstPersonComponentPtr fpComponent = new FirstPersonComponent();
	m_player->attachComponent(fpComponent);
	CameraComponentPtr cameraComponent = new CameraComponent();
	m_player->attachComponent(cameraComponent);
	m_player->attachComponent(new InputComponent());
	m_player->init(m_world);

	fpComponent->attachCamera(cameraComponent->getCamera());

	EntityPtr ogre = m_world.createEntity();
	ogre->attachComponent(new SceneComponent());
	//ModelComponent* model = new ModelComponent("ogrehead.mesh");
	//ogre->attachComponent(model);

	/*Ogre::Entity* e = m_scene->createEntity("OgreHead", "ogrehead.mesh");
	Ogre::SceneNode* n = m_scene->getRootSceneNode()->createChildSceneNode("head");
	n->attachObject(e);
	n->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));*/
	
}

// ========================================================================= //

void IntroState::exit(void)
{
	m_player->destroy(m_world);
	m_world.destroy(m_root);
}

// ========================================================================= //

void IntroState::update(void)
{
	m_world.update();
}

// ========================================================================= //