// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PositionComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements PositionComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

SceneComponent::SceneComponent(void) :
Component(),
m_node(nullptr)
{
	this->setName("SceneComponent");
}

// ========================================================================= //

SceneComponent::~SceneComponent(void)
{

}

// ========================================================================= //

void SceneComponent::init(EntityPtr entity, World& world)
{
	m_node = world.getSceneManager()->getRootSceneNode()->
		createChildSceneNode(toString(entity->getID()));
}

// ========================================================================= //

void SceneComponent::destroy(EntityPtr entity, World& world)
{
	world.getSceneManager()->destroySceneNode(m_node);
}

// ========================================================================= //

void SceneComponent::update(EntityPtr, World&)
{
	
}

// ========================================================================= //

void SceneComponent::message(const ComponentMessage& msg)
{

}

// ========================================================================= //

void SceneComponent::attachModel(const ModelComponentPtr modelC)
{
	m_node->attachObject(modelC->getOgreEntity());
}

// ========================================================================= //