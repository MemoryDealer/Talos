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
m_entity(nullptr)
{
    
}

// ========================================================================= //

ModelComponent::~ModelComponent(void)
{

}

// ========================================================================= //

void ModelComponent::init(void)
{
    
}

// ========================================================================= //

void ModelComponent::destroy(void)
{
    m_entity->detachFromParent();
    m_entity = nullptr;
}

// ========================================================================= //

void ModelComponent::update(void)
{

}

// ========================================================================= //

void ModelComponent::message(ComponentMessage& msg)
{

}

// ========================================================================= //

void ModelComponent::setMesh(const std::string& mesh,
                             const std::string& mat)
{
    // Create Ogre::Entity (assigns mesh).
    m_entity = this->getWorld()->getSceneManager()->createEntity(mesh);

    // Assign a material if specified.
    if (mat != ""){
        m_entity->setMaterialName(mat);
    }
}

// ========================================================================= //

// Getters:

// ========================================================================= //

Ogre::Entity* ModelComponent::getOgreEntity(void) const
{
    return m_entity;
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void ModelComponent::setMaterialName(const std::string& name){
    Assert(m_entity != nullptr,
           "ModelComponent::setMaterialName() call on null Ogre::Entity");

    m_entity->setMaterialName(name);
}

// ========================================================================= //