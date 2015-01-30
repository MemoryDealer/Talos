// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: FirstPersonComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements FirstPersonComponent class.
// ========================================================================= //

#include "FirstPersonComponent.hpp"
#include "Message.hpp"
#include "World/World.hpp"

// ========================================================================= //

FirstPersonComponent::FirstPersonComponent(void) :
m_yawNode(nullptr),
m_pitchNode(nullptr),
m_rollNode(nullptr)
{
	this->setName("FirstPersonComponent");
}

// ========================================================================= //

FirstPersonComponent::~FirstPersonComponent(void)
{

}

// ========================================================================= //

void FirstPersonComponent::init(EntityPtr entity, World& world)
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
}

// ========================================================================= //

void FirstPersonComponent::destroy(EntityPtr entity, World& world)
{
	world.getSceneManager()->destroySceneNode(m_yawNode);
	SceneComponent::destroy(entity, world);
}

// ========================================================================= //

void FirstPersonComponent::update(EntityPtr, World&)
{
	printf("pos: <%.2f, %.2f, %.2f>\n", m_cameraNode->getPosition().x,
		   m_cameraNode->getPosition().y,
		   m_cameraNode->getPosition().z);
}

// ========================================================================= //

void FirstPersonComponent::message(const Message& msg)
{
	switch (msg.type){
	default:
		break;

	case MessageType::INPUT_MOUSE_MOTION:
		m_yawNode->yaw(Ogre::Degree(Ogre::Real(msg.mouse.x)));
		m_pitchNode->pitch(Ogre::Degree(-Ogre::Real(msg.mouse.y)));
		break;
	}
}

// ========================================================================= //