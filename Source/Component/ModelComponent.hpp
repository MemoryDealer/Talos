// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: ModelComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ModelComponent class.
// ========================================================================= //

#ifndef __MODELCOMPONENT_HPP__
#define __MODELCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Needed information to render a 3D model in the game world.
class ModelComponent : public Component
{
public:
	// Default initializes member data, stores mesh filename.
	explicit ModelComponent(void);

	// Empty destructor.
	virtual ~ModelComponent(void) override;

	// Loads Ogre::Entity from scene manager using scene manager.
	virtual void init(EntityPtr, World&) override;

	// Empty.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

	// Getters:

	// Returns internal Ogre::Entity pointer.
	Ogre::Entity* getOgreEntity(void) const;

private:
	Ogre::Entity* m_entity;
	std::string m_meshFilename;
};

// ========================================================================= //

// Getters:

inline Ogre::Entity* ModelComponent::getOgreEntity(void) const{
	return m_entity;
}

// ========================================================================= //

#endif

// ========================================================================= //