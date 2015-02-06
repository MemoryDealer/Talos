// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: ActorComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements ActorComponent class.
// ========================================================================= //

#include "ActorComponent.hpp"
#include "ComponentMessage.hpp"
#include "World/World.hpp"

// ========================================================================= //

ActorComponent::ActorComponent(void) :
SceneComponent(),
m_yawNode(nullptr),
m_pitchNode(nullptr),
m_rollNode(nullptr),
m_translate(Ogre::Vector3::ZERO),
m_speed(1.f),
m_mode(Mode::SPECTATOR)
{
	this->setName("ActorComponent");
}

// ========================================================================= //

ActorComponent::~ActorComponent(void)
{

}

// ========================================================================= //

void ActorComponent::init(EntityPtr entity, World& world)
{
	SceneComponent::init(entity, world);

	// Acquire the camera node from the parent class.
	m_cameraNode = this->getSceneNode();

	// Create yaw node as the camera's top node.
	m_yawNode = m_cameraNode->createChildSceneNode();

	// Create pitch node as the camera's middle node.
	m_pitchNode = m_yawNode->createChildSceneNode();

	// Create roll node as the camera's bottom node.
	m_rollNode = m_pitchNode->createChildSceneNode();

	// Set to identity rotations.
	m_yawNode->setOrientation(Ogre::Quaternion::IDENTITY);
	m_pitchNode->setOrientation(Ogre::Quaternion::IDENTITY);
	m_rollNode->setOrientation(Ogre::Quaternion::IDENTITY);
}

// ========================================================================= //

void ActorComponent::destroy(EntityPtr entity, World& world)
{
	world.getSceneManager()->destroySceneNode(m_yawNode);
	SceneComponent::destroy(entity, world);
}

// ========================================================================= //

void ActorComponent::update(EntityPtr, World&)
{
	printf("pos: <%.2f, %.2f, %.2f>\n", m_cameraNode->getPosition().x,
		   m_cameraNode->getPosition().y,
		   m_cameraNode->getPosition().z);

	switch (m_mode){
	default:
		break;

	case Mode::SPECTATOR:
		m_cameraNode->translate(m_yawNode->getOrientation() *
								m_pitchNode->getOrientation() *
								m_translate,
								Ogre::SceneNode::TS_LOCAL);
	}
}

// ========================================================================= //

void ActorComponent::message(const ComponentMessage& msg)
{
	const Ogre::Real sens = 0.2f;

	switch (msg.type){
	default:
		break;

	case ComponentMessageType::INPUT_MOUSE_MOTION:
		m_yawNode->yaw(Ogre::Degree(-Ogre::Real(msg.mouse.x) * sens));
		m_pitchNode->pitch(Ogre::Degree(-Ogre::Real(msg.mouse.y) * sens));
		{
			// Prevent the camera from pitching upside down.
			Ogre::Real pitchAngle = 0.f;
			Ogre::Real pitchAngleParity = 0.f;

			// Get the angle of rotation around the x-axis.
			pitchAngle = (2.f * Ogre::Degree(Ogre::Math::ACos(
				m_pitchNode->getOrientation().w)).valueDegrees());

			pitchAngleParity = m_pitchNode->getOrientation().x;

			// Limit pitch.
			// @TODO: Define this in data.
			if (pitchAngle > 80.f){
				if (pitchAngleParity > 0){
					m_pitchNode->setOrientation(Ogre::Quaternion(
						Ogre::Math::Sqrt(0.5f),
						Ogre::Math::Sqrt(0.5f) - 0.115f,
						0.f,
						0.f));
				}
				else if (pitchAngleParity < 0){
					m_pitchNode->setOrientation(Ogre::Quaternion(
						Ogre::Math::Sqrt(0.5f),
						-Ogre::Math::Sqrt(0.5f) + 0.115f,
						0.f,
						0.f));
				}
			}
		}
		break;
	}
}

// ========================================================================= //

void ActorComponent::setMovingForward(const bool moving)
{
	m_translate.z = (moving) ? -m_speed : 0.f;
}

// ========================================================================= //

void ActorComponent::setMovingBackward(const bool moving)
{
	m_translate.z = (moving) ? m_speed : 0.f;
}

// ========================================================================= //

void ActorComponent::setMovingLeft(const bool moving)
{
	m_translate.x = (moving) ? -m_speed : 0.f;
}

// ========================================================================= //

void ActorComponent::setMovingRight(const bool moving)
{
	m_translate.x = (moving) ? m_speed : 0.f;
}

// ========================================================================= //