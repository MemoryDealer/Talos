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

#include "Component/ActorComponent.hpp"
#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/Entity.hpp"
#include "CC/KCC.hpp"
#include "PhysicsSystem.hpp"
#include "Physics/PScene.hpp"
#include "World/World.hpp"

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
    
    /*if (physicsC->isKinematic()){
        PxObstacleContext* oc = m_world->getPScene()->getControllerManager()->createObstacleContext();
        PxBoxObstacle bo;
        PxExtendedVec3 pos;
        pos.x = Physics::toPx(physicsC->getPosition()).x;
        pos.y = Physics::toPx(physicsC->getPosition()).y;
        pos.z = Physics::toPx(physicsC->getPosition()).z;
        bo.mPos = pos;
        bo.mRot = Physics::toPx(physicsC->getOrientation());
        bo.mHalfExtents = physicsC->getRigidActor()->getWorldBounds().getDimensions();
        m_oHandles[0] = oc->addObstacle(bo);
    }*/
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

            /*PxObstacleContext* oc = m_world->getPScene()->getControllerManager()->getObstacleContext(0);
            PxBoxObstacle* bo = static_cast<PxBoxObstacle*>(const_cast<PxObstacle*>(oc->getObstacle(0)));
            PxVec3 pos = Physics::toPx(physicsC->getPosition());
            PxExtendedVec3 epos(pos.x, pos.y, pos.z);
            bo->mPos = epos;
            bo->mRot = Physics::toPx(physicsC->getOrientation());
            PxTransform tr = m_world->getPlayer()->getComponent<ActorComponent>()->getKCC()->m_controller->getActor()->getGlobalPose();
            
            oc->updateObstacle(m_oHandles[0], *bo);*/
        }
        else{
            sceneC->setPosition(physicsC->getPosition());
            sceneC->setOrientation(physicsC->getOrientation());
        }
    }
}

// ========================================================================= //