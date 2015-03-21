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

#include "ComponentMessage.hpp"
#include "LightComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

LightComponent::LightComponent(void) :
m_light(nullptr),
m_intensity(1.f),
m_type(Type::Point)
{
    
}

// ========================================================================= //

LightComponent::~LightComponent(void)
{

}

// ========================================================================= //

void LightComponent::init(void)
{
    m_light = this->getWorld()->getSceneManager()->createLight();
    m_light->setCastShadows(true);

    // Set to point light by default.
    m_light->setType(Ogre::Light::LT_POINT);

    // Set default range of 50.f.
    this->setRange(50.f);
}

// ========================================================================= //

void LightComponent::destroy(void)
{
    this->getWorld()->getSceneManager()->destroyLight(m_light);
}

// ========================================================================= //

void LightComponent::update(void)
{

}

// ========================================================================= //

void LightComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::Command:
        if (boost::get<CommandType>(msg.data) == CommandType::Flashlight){
            // Switch the light on/off.
            m_light->setVisible(!m_light->getVisible());
        }
        break;
    }
}

// ========================================================================= //

// Getters:

// ========================================================================= //

const LightComponent::Type& LightComponent::getType(void) const
{
    return m_type;
}

// ========================================================================= //

Ogre::Light* LightComponent::getLight(void) const
{
    return m_light;
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void LightComponent::setType(const Type& type)
{
    m_light->setType((type == Type::Point) ? Ogre::Light::LT_POINT :
                     Ogre::Light::LT_SPOTLIGHT);
    
    m_type = type;

    if (m_type == Type::Spotlight){
        m_light->setCastShadows(true);
    }
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
        
        m_light->setAttenuation(range, 1.f, 1.f, 1.f);
        m_light->setSpotlightRange(Ogre::Degree(5.f), Ogre::Degree(21.f));
        //m_light->setSpotlightFalloff(5.f);
    }
}

// ========================================================================= //

void LightComponent::setEnabled(const bool enabled)
{
    m_light->setVisible(enabled);
}

// ========================================================================= //