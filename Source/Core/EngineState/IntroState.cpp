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

	m_camera = m_scene->createCamera("IntroCam");
	m_camera->setPosition(Ogre::Vector3::ZERO);
	m_camera->lookAt(Ogre::Vector3(0.0, 0.0, 50.0));
	m_camera->setNearClipDistance(1.0);
	m_camera->setAspectRatio(Ogre::Real(m_viewport->getActualWidth()) /
		Ogre::Real(m_viewport->getActualHeight()));

	m_viewport->setCamera(m_camera);
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
	static bool s = false;
	if (s){
		m_viewport->setBackgroundColour(Ogre::ColourValue::Green);
		s = false;
	}
	else{
		m_viewport->setBackgroundColour(Ogre::ColourValue::Red);
		s = true;
	}
}

// ========================================================================= //