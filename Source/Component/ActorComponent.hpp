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

using namespace physx;

// ========================================================================= //

class DCC;
class KCC;

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

	// Two types of character controllers.
	enum CC{
		KINEMATIC = 0,
		DYNAMIC
	};

	// Component functions:

	// Attachs a camera to the roll node.
	void attachCamera(const CameraComponentPtr);

	// Changes the actor's orientation based on relative x/y looking.
	void look(const int, const int);

	// Getters:

	const bool isMovingForward(void) const;

	const bool isMovingBack(void) const;

	const bool isMovingLeft(void) const;

	const bool isMovingRight(void) const;

	// Setters:

	// Sets the mode of the Actor, affecting controls/movement.
	void setMode(const Mode);

	// Sets translate vector to forward if in spectator mode. 
	void setMovingForward(const bool);

	// Sets translate vector to backward if in spectator mode. 
	void setMovingBack(const bool);

	// Sets translate vector to left if in spectator mode. 
	void setMovingLeft(const bool);

	// Sets translate vector to right if in spectator mode. 
	void setMovingRight(const bool);

private:
	// Ogre3D.
	Ogre::Camera* m_camera;
	Ogre::SceneNode* m_cameraNode;
	Ogre::SceneNode* m_yawNode;
	Ogre::SceneNode* m_pitchNode;
	Ogre::SceneNode* m_rollNode;
	Ogre::Real m_speed;

	// Character controller.
	CC m_cc;
	union{
		DCC* m_dcc;
		KCC* m_kcc;
	};

	Mode m_mode;
	bool m_movingForward;
	bool m_movingBack;
	bool m_movingLeft;
	bool m_movingRight;
};

// ========================================================================= //

// Getters:

inline const bool ActorComponent::isMovingForward(void) const{
	return m_movingForward;
}

inline const bool ActorComponent::isMovingBack(void) const{
	return m_movingBack;
}

inline const bool ActorComponent::isMovingLeft(void) const{
	return m_movingLeft;
}

inline const bool ActorComponent::isMovingRight(void) const{
	return m_movingRight;
}

// Setters:

inline void ActorComponent::setMovingForward(const bool moving){
	m_movingForward = moving;
}

inline void ActorComponent::setMovingBack(const bool moving){
	m_movingBack = moving;
}

inline void ActorComponent::setMovingLeft(const bool moving){
	m_movingLeft = moving;
}

inline void ActorComponent::setMovingRight(const bool moving){
	m_movingRight = moving;
}

// ========================================================================= //

#endif

// ========================================================================= //