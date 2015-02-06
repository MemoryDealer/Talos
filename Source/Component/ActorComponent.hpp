// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: ActorComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ActorComponent class.
// ========================================================================= //

#ifndef __ACTORCOMPONENT_HPP__
#define __ACTORCOMPONENT_HPP__

// ========================================================================= //

#include "SceneComponent.hpp"

// ========================================================================= //
// Allows an Entity to be controlled by either: a player giving local input,
// an AI, a networked player, or from a list of replay commands.
class ActorComponent final : public SceneComponent
{
public:
	// Default initializes scene nodes.
	explicit ActorComponent(void);

	// Empty destructor.
	virtual ~ActorComponent(void) override;

	// Creates all needed scene nodes for controlling first person camera.
	virtual void init(EntityPtr, World&) override;

	// Destroys the internal scene nodes for the camera.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Handles input messages.
	virtual void message(const ComponentMessage&) override;

	// Modes the actor can be in.
	enum Mode{
		SPECTATOR = 0,
		PLAYER
	};

	// Attachs a camera to the roll node.
	void attachCamera(Ogre::Camera* camera);

	// Getters:

	// ...

	// Setters:

	// Sets the mode of the Actor, affecting controls/movement.
	void setMode(const Mode);

	// Sets translate vector to forward if in spectator mode. 
	void setMovingForward(const bool);

	// Sets translate vector to backward if in spectator mode. 
	void setMovingBackward(const bool);

	// Sets translate vector to left if in spectator mode. 
	void setMovingLeft(const bool);

	// Sets translate vector to right if in spectator mode. 
	void setMovingRight(const bool);

private:
	Ogre::SceneNode* m_cameraNode;
	Ogre::SceneNode* m_yawNode;
	Ogre::SceneNode* m_pitchNode;
	Ogre::SceneNode* m_rollNode;

	Ogre::Vector3 m_translate;
	Ogre::Real m_speed;

	Mode m_mode;
};

// ========================================================================= //

inline void ActorComponent::attachCamera(Ogre::Camera* camera){
	m_rollNode->attachObject(camera);
}

// Setters:

inline void ActorComponent::setMode(const Mode mode){
	m_mode = mode;
}

// ========================================================================= //

#endif

// ========================================================================= //