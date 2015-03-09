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
// File: CollisionComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements CollisionComponent class.
// ========================================================================= //

#include "CollisionComponent.hpp"
#include "Entity/Entity.hpp"
#include "ModelComponent.hpp"
#include "Physics/PScene.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

CollisionComponent::CollisionComponent(void) :
m_rigidActor(nullptr),
m_type(Type::Box),
m_entityID(0)
{

}

// ========================================================================= //

CollisionComponent::~CollisionComponent(void)
{

}

// ========================================================================= //

void CollisionComponent::init(World& world)
{
    this->setInitialized(true);
}

// ========================================================================= //

void CollisionComponent::init(World& world, EntityPtr entity)
{
    // Get the Ogre::Entity for mesh data.
    Ogre::Entity* e = entity->getComponent<ModelComponent>()->getOgreEntity();
    PxVec3 pos(Physics::toPx(
        entity->getComponent<SceneComponent>()->getPosition()));
    std::shared_ptr<PxGeometry> geometry;

    // Determine type of collision volume to create.
    switch (m_type){
    default:
    case Type::Box:
        {
            // Get the bounding box half extents.
            PxVec3 v = Physics::toPx(e->getBoundingBox().getHalfSize());
            // Ensure not dimensions are zero.
            const PxReal min = 0.01f;
            if (v.x == 0.f){
                v.x = min;
            }
            if (v.y == 0.f){
                v.y = min;
            }
            if (v.z == 0.f){
                v.z = min;
            }
            // Create PhysX geometry.
            geometry.reset(new PxBoxGeometry(v));
        }
        break;

    case Type::Mesh:

        break;

    case Type::Plane:
        /*PxRigidStatic* p = PxCreatePlane(*m_world.getPScene()->m_physx,
        PxPlane(PxVec3(0.f, 1.f, 0.f), 50.f),
        *mat);*/
        break;
    }

    // Create PhysX static actor with geometry.
    m_rigidActor = PxCreateStatic(*world.getPScene()->getSDK(),
                                  PxTransform(pos),
                                  *geometry,
                                  *world.getPScene()->getDefaultMaterial());

    // Assign actor's user data to EntityID.
    m_rigidActor->userData = reinterpret_cast<void*>(
        static_cast<const EntityID>(entity->getID()));

    // Add actor to PhysX scene.
    world.getPScene()->getScene()->addActor(*m_rigidActor);

    // Add to debug drawer if activated.
    if (world.getPScene()->isUsingDebugDrawer()){
        world.getPScene()->addToDebugDrawer(m_rigidActor, *geometry);
    }
}

// ========================================================================= //

void CollisionComponent::destroy(World& world)
{
    Assert(m_rigidActor != nullptr, "Null m_actor!");

    world.getPScene()->getScene()->removeActor(*m_rigidActor);

    this->setInitialized(false);
}

// ========================================================================= //

void CollisionComponent::update(World& world)
{

}

// ========================================================================= //

void CollisionComponent::message(const ComponentMessage& msg)
{

}

// ========================================================================= //


const Ogre::Vector3 CollisionComponent::getPosition(void) const
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    return Ogre::Vector3(transform.p.x,
                         transform.p.y,
                         transform.p.z);
}

// ========================================================================= //

const Ogre::Quaternion CollisionComponent::getOrientation(void) const
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    return Ogre::Quaternion(transform.q.w,
                            transform.q.x,
                            transform.q.y,
                            transform.q.z);
}

// ========================================================================= //