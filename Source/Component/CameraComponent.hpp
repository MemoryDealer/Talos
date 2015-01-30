// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: CameraComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines CameraComponent class.
// ========================================================================= //

#ifndef __CAMERACOMPONENT_HPP__
#define __CAMERACOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Holds an Ogre::Camera to view the game world.
class CameraComponent : public Component
{
public:
	// Sets name to "CameraComponent".
	explicit CameraComponent(void);

	// Empty destructor.
	virtual ~CameraComponent(void) override;

	// Creates Ogre::Camera object, sets its clip distance, sets the aspect
	// ratio, wires up the camera to the viewport, and attachs the camera
	// object to the SceneComponent of the parent Entity if it exists.
	virtual void init(EntityPtr, World&) override;

	// Destroys the Ogre::Camera object.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

private:
	Ogre::Camera* m_camera;
};

// ========================================================================= //

#endif

// ========================================================================= //