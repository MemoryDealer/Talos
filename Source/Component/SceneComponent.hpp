// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PositionComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines PositionComponent class.
// ========================================================================= //

#ifndef __SCENECOMPONENT_HPP__
#define __SCENECOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Holds information for position in the game world.
class SceneComponent : public Component
{
public:
	// Default initializes Ogre::SceneNode.
	explicit SceneComponent(void);

	// Empty destructor.
	virtual ~SceneComponent(void) override;

	virtual void init(EntityPtr, World&) override;

	virtual void destroy(EntityPtr, World&) override;

	virtual void update(EntityPtr, World&) override;

	void attachObject(Ogre::MovableObject*);

	//Ogre::SceneNode* getSceneNode(void) const;

private:
	Ogre::SceneNode* m_node;
};

// ========================================================================= //

inline void SceneComponent::attachObject(Ogre::MovableObject* object){
	m_node->attachObject(object);
}

// ========================================================================= //

#endif

// ========================================================================= //