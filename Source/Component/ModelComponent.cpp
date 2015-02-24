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
Component(),
m_entity(nullptr)
{
    
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
    m_entity->detachFromParent();
    m_entity = nullptr;

    this->setInitialized(false);
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