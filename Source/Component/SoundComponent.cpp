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
// File: SoundComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements SoundComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "SoundComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

SoundComponent::SoundComponent(void) :
m_sound(nullptr),
m_node(nullptr),
m_looped(true)
{

}

// ========================================================================= //

SoundComponent::~SoundComponent(void)
{

}

// ========================================================================= //

void SoundComponent::destroy(void)
{
    if (m_sound){
        m_sound->drop();
    }
}

// ========================================================================= //

void SoundComponent::update(void)
{
    // Update 3D position of sound.
    if (m_sound){
        Ogre::Vector3 pos = m_node->_getDerivedPosition();

        m_sound->setPosition(irrklang::vec3df(pos.x, pos.y, pos.z));

        if (m_sound->isFinished()){
            m_sound->drop();
            m_sound = nullptr;
        }
    }    
}

// ========================================================================= //

void SoundComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::Action:
        if (!m_looped){
            if (m_sound){
                m_sound->setIsPaused(false);
            }
        }
        break;
    }
}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void SoundComponent::addSound(const std::string& file, const bool looped)
{
    m_looped = looped;

    irrklang::ISoundEngine* soundEngine = this->getWorld()->getSoundEngine();
    irrklang::vec3df pos(0.f, 0.f, 0.f);

    m_sound = soundEngine->play3D(file.c_str(),
                                  pos,
                                  m_looped,
                                  !m_looped,
                                  true);
}

// ========================================================================= //

void SoundComponent::setSceneNode(Ogre::SceneNode* node)
{
    m_node = node;
}

// ========================================================================= //