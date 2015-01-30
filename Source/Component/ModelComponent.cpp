// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: GraphicsComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements GraphicsComponent class.
// ========================================================================= //

#include "ModelComponent.hpp"

// ========================================================================= //

ModelComponent::ModelComponent(void) :
m_entity(nullptr)
{
	this->setName("ModelComponent");
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

void ModelComponent::destroy(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void ModelComponent::update(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void ModelComponent::message(const Message& msg)
{

}

// ========================================================================= //