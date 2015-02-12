// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: LightComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements LightComponent class.
// ========================================================================= //

#include "LightComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

LightComponent::LightComponent(void) :
m_light(nullptr)
{
	this->setType(Component::Type::Light);
}

// ========================================================================= //

LightComponent::~LightComponent(void)
{

}

// ========================================================================= //

void LightComponent::init(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void LightComponent::init(World& world, const Type type)
{
	m_light = world.getSceneManager()->createLight();
	m_light->setType((type == Type::POINT) ? Ogre::Light::LT_POINT :
					 Ogre::Light::LT_SPOTLIGHT);

	this->setInitialized(true);
}

// ========================================================================= //

void LightComponent::destroy(EntityPtr entity, World& world)
{
	world.getSceneManager()->destroyLight(m_light);
}

// ========================================================================= //

void LightComponent::update(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void LightComponent::message(const ComponentMessage&)
{

}

// ========================================================================= //