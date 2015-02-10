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
#include "Physics/PScene.hpp"
#include "World/World.hpp"

// ========================================================================= //

ActorComponent::ActorComponent(void) :
SceneComponent(),
m_yawNode(nullptr),
m_pitchNode(nullptr),
m_rollNode(nullptr),
m_translate(Ogre::Vector3::ZERO),
m_speed(0.5f),
m_mode(Mode::PLAYER),
m_pxController(nullptr)
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

	// Create PhysX character controller.
	PxCapsuleControllerDesc desc;
	desc.position = PxExtendedVec3(0.f, 0.f, 0.f);
	desc.height = 1.8288f;
	desc.radius = 0.3048f;
	PxMaterial* mat = world.getPScene()->getSDK()->createMaterial(0.5f,
																  0.5f,
																  0.1f);
	desc.material = mat;
	m_pxController = world.getPScene()->getControllerManager()->
		createController(desc);
	m_pxController->getActor()->setMass(80.f);
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
	switch (m_mode){
	default:
		break;

	case Mode::SPECTATOR:
		m_cameraNode->translate(m_yawNode->getOrientation() *
								m_pitchNode->getOrientation() *
								m_translate,
								Ogre::SceneNode::TS_LOCAL);
		break;

	case Mode::PLAYER:
		{
			Ogre::Vector3 translate = m_yawNode->getOrientation() *
				m_pitchNode->getOrientation() *
				m_translate;
			const PxReal gravity = -9.81f / 16.f;
			PxVec3 disp(translate.x, gravity, translate.z);
			PxU32 flags = m_pxController->move(disp,
											   0.001f,
											   16.f,
											   0);
			PxExtendedVec3 pos = m_pxController->getPosition();
			m_cameraNode->setPosition(Ogre::Real(pos.x), 
									Ogre::Real(pos.y) + 0.8644f, 
									Ogre::Real(pos.z));

			
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
	m_rollNode->attachObject(cameraC->getCamera());
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