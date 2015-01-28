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

	m_scene = m_root->createSceneManager(Ogre::ST_GENERIC, "IntroSceneManager");
	m_scene->setAmbientLight(Ogre::ColourValue::White);

	m_scene->setSkyDome(true, "Clouds");

	
	m_camera = m_scene->createCamera("IntroCam");
	m_camera->setNearClipDistance(1.0);
	m_camera->setAspectRatio(Ogre::Real(m_viewport->getActualWidth()) /
		Ogre::Real(m_viewport->getActualHeight()));
	m_viewport->setCamera(m_camera);

	m_cameraNode = m_scene->getRootSceneNode()->createChildSceneNode("CameraNode");
	m_cameraNode->attachObject(m_camera);
	m_cameraNode->setPosition(Ogre::Vector3(0.0, 50.0, 50.0));

	Ogre::Entity* e = m_scene->createEntity("OgreHead", "ogrehead.mesh");
	Ogre::SceneNode* n = m_scene->getRootSceneNode()->createChildSceneNode("head");
	n->attachObject(e);
	n->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));
}

// ========================================================================= //

void IntroState::exit(void)
{
	m_scene->destroyCamera(m_camera);
	m_root->destroySceneManager(m_scene);
}

// ========================================================================= //

void IntroState::update(void)
{
	m_cameraNode->pitch(Ogre::Degree(1.0));
	m_scene->getSceneNode("head")->yaw(Ogre::Degree(1.0));
}

// ========================================================================= //