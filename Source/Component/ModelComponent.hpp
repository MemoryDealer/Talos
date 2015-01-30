// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: GraphicsComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines GraphicsComponent class.
// ========================================================================= //

#ifndef __MODELCOMPONENT_HPP__
#define __MODELCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Needed information to render a 2D or 3D object.
class ModelComponent : public Component
{
public:
	explicit ModelComponent(void);
	virtual ~ModelComponent(void) override;

	virtual void init(EntityPtr, World&) override;

	virtual void destroy(EntityPtr, World&) override;

	virtual void update(EntityPtr, World&) override;

	virtual void message(const Message&) override;

private:
	Ogre::Entity* m_entity;
};

// ========================================================================= //

#endif

// ========================================================================= //