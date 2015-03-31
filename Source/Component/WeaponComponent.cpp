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
// File: WeaponComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements WeaponComponent class.
// ========================================================================= //

#include "WeaponComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

WeaponComponent::WeaponComponent(void) :
m_node(nullptr),
m_entity(nullptr),
m_clearDepth(false)
{

}

// ========================================================================= //

WeaponComponent::~WeaponComponent(void)
{

}

// ========================================================================= //

void WeaponComponent::init(void)
{

}

// ========================================================================= //

void WeaponComponent::destroy(void)
{

}

// ========================================================================= //

void WeaponComponent::update(void)
{

}

// ========================================================================= //

void WeaponComponent::message(ComponentMessage& msg)
{

}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void WeaponComponent::setup(Ogre::SceneNode* actorRollNode)
{
    m_node = actorRollNode->createChildSceneNode();
    m_entity = this->getWorld()->getSceneManager()->createEntity("laserrifle.mesh");
    m_entity->setMaterialName("DiffuseBlack");    
    m_node->attachObject(m_entity);

    m_node->translate(0.9f, -0.6f, -6.0f);
    m_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(-90.f));
    m_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(180.f));

    

    // Setup node for render listener, so the weapon renders last.
    if (m_clearDepth){
        m_entity->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_9);
    }
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void WeaponComponent::setClearDepth(const bool clear)
{
    m_clearDepth = clear;
}

// ========================================================================= //