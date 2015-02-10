// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: CameraComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements CameraComponent class.
// ========================================================================= //

#include "CameraComponent.hpp"
#include "ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

CameraComponent::CameraComponent(void) :
Component(),
m_camera(nullptr)
{
	this->setName("CameraComponent");
}

// ========================================================================= //

CameraComponent::~CameraComponent(void)
{

}

// ========================================================================= //

void CameraComponent::init(EntityPtr entity, World& world)
{
	// Create the camera object.
	m_camera = world.getSceneManager()->createCamera("PlayerCam");
	m_camera->setNearClipDistance(0.1f);

	// Setup viewport aspect ratio and assign camera to viewport.
	Ogre::Viewport* viewport = world.getViewport();
	m_camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) /
							 Ogre::Real(viewport->getActualHeight()));
	viewport->setCamera(m_camera);

	this->setInitialized(true);
}

// ========================================================================= //

void CameraComponent::destroy(EntityPtr entity, World& world)
{
	world.getSceneManager()->destroyCamera(m_camera);
}

// ========================================================================= //

void CameraComponent::update(EntityPtr entity, World& world)
{
	
}

// ========================================================================= //

void CameraComponent::message(const ComponentMessage& msg)
{

}

// ========================================================================= //