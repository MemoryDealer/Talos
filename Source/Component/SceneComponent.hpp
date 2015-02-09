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

	// Creates a Ogre::SceneNode within the world.
	virtual void init(EntityPtr, World&) override;

	// Destroys the internal Ogre::SceneNode.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

	// Component functions:

	// Joins a ModelComponent to itself.
	void attachModel(const ModelComponentPtr);

	// Sets position of Ogre::SceneNode. 
	// Note: This may be overriden by a PhysicsComponent attached to the
	// same Entity.
	void setPosition(const Ogre::Real, const Ogre::Real, const Ogre::Real);

	// Sets orientation of Ogre::SceneNode.
	void setOrientation(const Ogre::Real, const Ogre::Real, const Ogre::Real,
						const Ogre::Real);

	// Getters:

	// Returns pointer to internal Ogre::SceneNode.
	Ogre::SceneNode* getSceneNode(void) const;

private:
	Ogre::SceneNode* m_node;
};

// ========================================================================= //

inline Ogre::SceneNode* SceneComponent::getSceneNode(void) const{
	return m_node;
}

// ========================================================================= //

#endif

// ========================================================================= //