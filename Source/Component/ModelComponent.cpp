// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: ModelComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements ModelComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "ModelComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

ModelComponent::ModelComponent(void) :
Component(),
m_entity(nullptr)
{
	this->setType(Component::Type::Model);
}

// ========================================================================= //

ModelComponent::~ModelComponent(void)
{

}

// ========================================================================= //

void ModelComponent::init(EntityPtr entity, World& world)
{
	
}

// ========================================================================= //

void ModelComponent::init(World& world,
						  const std::string& meshName,
						  const std::string& matName)
{
	m_entity = world.getSceneManager()->createEntity(meshName);
	if (matName != ""){
		m_entity->setMaterialName(matName);
	}

	this->setInitialized(true);
}

// ========================================================================= //

void ModelComponent::destroy(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void ModelComponent::update(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void ModelComponent::message(const ComponentMessage& msg)
{

}

// ========================================================================= //