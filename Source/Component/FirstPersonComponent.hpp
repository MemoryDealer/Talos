// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: FirstPersonComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines FirstPersonComponent class.
// ========================================================================= //

#ifndef __FIRSTPERSONCOMPONENT_HPP__
#define __FIRSTPERSONCOMPONENT_HPP__

// ========================================================================= //

#include "SceneComponent.hpp"

// ========================================================================= //
// Holds data for operating a first person character, a CameraComponent must
// be used in conjuction with this.
class FirstPersonComponent final : public SceneComponent
{
public:
	// Default initializes scene nodes.
	explicit FirstPersonComponent(void);

	// Empty destructor.
	virtual ~FirstPersonComponent(void) override;

	// Creates all needed scene nodes for controlling first person camera.
	virtual void init(EntityPtr, World&) override;

	// Destroys the internal scene nodes for the camera.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Handles input messages.
	virtual void message(const Message&) override;

	// Attachs a camera to the roll node.
	void attachCamera(Ogre::Camera* camera);

private:
	Ogre::SceneNode* m_cameraNode;
	Ogre::SceneNode* m_yawNode;
	Ogre::SceneNode* m_pitchNode;
	Ogre::SceneNode* m_rollNode;
};

// ========================================================================= //

inline void FirstPersonComponent::attachCamera(Ogre::Camera* camera){
	m_rollNode->attachObject(camera);
}

// ========================================================================= //

#endif

// ========================================================================= //