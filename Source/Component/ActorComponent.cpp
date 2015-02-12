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
#include "CameraComponent.hpp"
#include "CC/DCC.hpp"
#include "CC/KCC.hpp"
#include "ComponentMessage.hpp"
#include "Physics/PScene.hpp"
#include "World/World.hpp"

// ========================================================================= //

ActorComponent::ActorComponent(void) :
SceneComponent(),
m_camera(nullptr),
m_cameraNode(nullptr),
m_yawNode(nullptr),
m_pitchNode(nullptr),
m_rollNode(nullptr),
m_speed(0.5f),
m_cc(CC::KINEMATIC),
m_dcc(nullptr),
m_kcc(nullptr),
m_mode(Mode::PLAYER),
m_movingForward(false),
m_movingBack(false),
m_movingLeft(false),
m_movingRight(false)
{
	this->setType(Component::Type::Actor);
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

	// Create PhysX character controller.
	if (m_cc == CC::KINEMATIC){
		m_kcc = new KCC();
		m_kcc->init(world);
	}

	this->setInitialized(true);
}

// ========================================================================= //

void ActorComponent::destroy(EntityPtr entity, World& world)
{
	// Destroy character controller.
	if (m_cc == CC::KINEMATIC){
		delete m_kcc;
	}
	else if (m_cc == CC::DYNAMIC){
		delete m_dcc;
	}

	world.getSceneManager()->destroySceneNode(m_yawNode);
	SceneComponent::destroy(entity, world);
}

// ========================================================================= //

void ActorComponent::update(EntityPtr, World&)
{
	Ogre::Vector3 translate(Ogre::Vector3::ZERO);
	if (m_movingForward){
		translate.z = -m_speed;
	}
	else if (m_movingBack){
		translate.z = m_speed;
	}
	if (m_movingLeft){
		translate.x = -m_speed;
	}
	else if (m_movingRight){
		translate.x = m_speed;
	}

	// Calculate movement vector.
	translate = m_yawNode->getOrientation() *
		m_pitchNode->getOrientation() *
		translate;
	// Calculate the forwards vector and use it to keep the player moving at
	// the same velocity despite the pitch of the camera.
	Ogre::Vector3 forwards = m_camera->getDerivedUp();
	forwards.crossProduct(m_camera->getDerivedRight());
	forwards.normalise();
	// Modify original movement vector.
	translate.x /= forwards.y;
	translate.z /= forwards.y;

	// Prevent faster movement when moving diagonally.
	translate.normalise();

	switch (m_mode){
	default:
		break;

	case Mode::SPECTATOR:
		m_cameraNode->translate(translate, Ogre::SceneNode::TS_LOCAL);
		break;

	case Mode::PLAYER:
		{
			if (m_cc == CC::KINEMATIC){
				PxExtendedVec3 pos = m_kcc->update(translate.x,
												   translate.y,
												   translate.z);
				m_cameraNode->setPosition(Ogre::Real(pos.x),
										  Ogre::Real(pos.y),
										  Ogre::Real(pos.z));
			}
		}
		break;
	}
}

// ========================================================================= //

void ActorComponent::message(const ComponentMessage& msg)
{
	
}

// ========================================================================= //

void ActorComponent::attachCamera(const CameraComponentPtr cameraC)
{
	m_camera = cameraC->getCamera();
	m_rollNode->attachObject(m_camera);
}

// ========================================================================= //

void ActorComponent::look(const int relx, const int rely)
{
	const Ogre::Real sens = 0.2f;

	m_yawNode->yaw(Ogre::Degree(-Ogre::Real(relx) * sens));
	m_pitchNode->pitch(Ogre::Degree(-Ogre::Real(rely) * sens));
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
}

// ========================================================================= //

void ActorComponent::setMode(const Mode mode)
{
	m_mode = mode;

	switch (m_mode){
	default:
		break;

	case Mode::PLAYER:
		// For debugging.
		m_kcc->setPosition(m_cameraNode->getPosition());
		break;

	case Mode::SPECTATOR:

		break;
	}
}

// ========================================================================= //