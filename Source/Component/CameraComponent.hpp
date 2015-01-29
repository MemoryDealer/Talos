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

class CameraComponent : public Component
{
public:
	explicit CameraComponent(void);

	virtual ~CameraComponent(void) override;

	void init(World& world);

	void destroy(World& world);

	void update(void);

private:
	Ogre::Camera* m_camera;
};

// ========================================================================= //

#endif

// ========================================================================= //