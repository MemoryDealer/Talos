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
// File: PhysicsSystem.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements PhysicsSystem class.
// ========================================================================= //

#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/Entity.hpp"
#include "PhysicsSystem.hpp"

// ========================================================================= //

PhysicsSystem::PhysicsSystem(void)
{

}

// ========================================================================= //

PhysicsSystem::~PhysicsSystem(void)
{

}

// ========================================================================= //

void PhysicsSystem::onEntityAttached(EntityPtr entity)
{
    PhysicsComponentPtr physicsC =
        entity->getComponent<PhysicsComponent>();

    physicsC->init(entity);
}

// ========================================================================= //

void PhysicsSystem::update(void)
{
    for (auto& i : m_entities){
        PhysicsComponentPtr physicsC = 
            i.second->getComponent<PhysicsComponent>();
        SceneComponentPtr sceneC =
            i.second->getComponent<SceneComponent>();

        // Update SceneComponent's position and orientaiton.
        if (physicsC->isKinematic()){
            physicsC->setPosition(sceneC->getPosition());
            physicsC->setOrientation(sceneC->getOrientation());
        }
        else{
            sceneC->setPosition(physicsC->getPosition());
            sceneC->setOrientation(physicsC->getOrientation());
        }
    }
}

// ========================================================================= //