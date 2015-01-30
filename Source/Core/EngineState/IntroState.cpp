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

#include "IntroState.hpp"
#include "World/World.hpp"
#include "Entity/Player.hpp"

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

	m_player = new Player();
	m_player->init(m_world);


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
	m_player->update(m_world);
}

// ========================================================================= //