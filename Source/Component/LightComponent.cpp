// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
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