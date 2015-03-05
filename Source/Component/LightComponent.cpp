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
    
}

// ========================================================================= //

LightComponent::~LightComponent(void)
{

}

// ========================================================================= //

void LightComponent::init(World& world)
{
    m_light = world.getSceneManager()->createLight();
    // Set to point light by default.
    m_light->setType(Ogre::Light::LT_POINT);
    this->setInitialized(true);
}

// ========================================================================= //

void LightComponent::destroy(World& world)
{
    world.getSceneManager()->destroyLight(m_light);

    this->setInitialized(false);
}

// ========================================================================= //

void LightComponent::update(World& world)
{

}

// ========================================================================= //

void LightComponent::message(const ComponentMessage&)
{

}

// ========================================================================= //

void LightComponent::setType(const Type type)
{
    m_light->setType((type == Type::POINT) ? Ogre::Light::LT_POINT :
                     Ogre::Light::LT_SPOTLIGHT);
}

// ========================================================================= //

void LightComponent::setColour(const Ogre::Real r,
                               const Ogre::Real g,
                               const Ogre::Real b)
{
    m_light->setDiffuseColour(r, g, b);
    m_light->setSpecularColour(r, g, b);
}

// ========================================================================= //

void LightComponent::setRange(const Ogre::Real range)
{
    if (m_light->getType() == Ogre::Light::LT_POINT){
        m_light->setAttenuation(range,
                                1.f,
                                //4.5f / range,
                                //75.f / (range * range));
                                255.f / range,
                                0.f);
    }
    else{
        m_light->setSpotlightRange(Ogre::Degree(range - 15),
                                   Ogre::Degree(range));
    }
}

// ========================================================================= //