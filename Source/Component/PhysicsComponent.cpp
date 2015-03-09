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
// File: PhysicsComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements StaticPhysicsComponent class.
// ========================================================================= //

#include "Entity/Entity.hpp"
#include "ModelComponent.hpp"
#include "Physics/PScene.hpp"
#include "PhysicsComponent.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

PhysicsComponent::PhysicsComponent(void) :
m_rigidActor(nullptr),
m_type(Type::Box),
m_mat(nullptr),
m_density(1.f)
{
    
}

// ========================================================================= //

PhysicsComponent::~PhysicsComponent(void)
{

}

// ========================================================================= //

void PhysicsComponent::init(World&)
{
    this->setInitialized(true);
}

// ========================================================================= //

void PhysicsComponent::init(World& world, EntityPtr entity)
{
    // If no custom material has been assigned, use the default.
    if (!m_mat){
        m_mat = world.getPScene()->getDefaultMaterial();
    }

    // Get Ogre::Entity for mesh data.
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
            PxVec3 v(Physics::toPx(e->getBoundingBox().getHalfSize()));
            // Ensure not dimensions are zero.
            const PxReal min = 0.001f;
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
            //Assert(geometry->isValid(), "Invalid PxBoxGeometry for PhysicsComponent");
        }
        break;

    case Type::Sphere:
        {
            // Create sphere geometry.
            geometry.reset(new PxSphereGeometry(5.f));
            //Assert(geometry->isValid(), "Invalid PxSphereGeometry for PhysicsComponent");
        }
        break;

    case Type::Mesh:

        break;
    }

    // Create dynamic actor.
    m_rigidActor = PxCreateDynamic(*world.getPScene()->getSDK(),
                                   PxTransform(pos),
                                   *geometry,
                                   *m_mat,
                                   m_density);

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

void PhysicsComponent::destroy(World& world)
{
    Assert(m_rigidActor, "Null m_actor!");

    world.getPScene()->getScene()->removeActor(*m_rigidActor);

    this->setInitialized(false);
}

// ========================================================================= //

void PhysicsComponent::update(World& world)
{
    
}

// ========================================================================= //

void PhysicsComponent::message(const ComponentMessage&)
{

}

// ========================================================================= //

void PhysicsComponent::setPosition(const PxReal x, 
                                   const PxReal y , 
                                   const PxReal z)
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    transform.p.x = x;
    transform.p.y = y;
    transform.p.z = z;

    m_rigidActor->setGlobalPose(transform, true);
}

// ========================================================================= //

// Sets PhysX pose orientation to this quaternion.
void PhysicsComponent::setOrientation(const PxReal w, 
                                      const PxReal x, 
                                      const PxReal y,
                                      const PxReal z)
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    transform.q.w = w;
    transform.q.x = x;
    transform.q.y = y;
    transform.q.z = z;

    m_rigidActor->setGlobalPose(transform, true);
}

// ========================================================================= //

void PhysicsComponent::translate(const PxReal dx, 
                                 const PxReal dy, 
                                 const PxReal dz)
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    transform.p.x += dx;
    transform.p.y += dy;
    transform.p.z += dz;

    m_rigidActor->setGlobalPose(transform, true);
}

// ========================================================================= //

void PhysicsComponent::rotate(const PxReal dx,
                              const PxReal dy,
                              const PxReal dz)
{
    // @TODO: This doesn't worK!
    PxTransform transform = m_rigidActor->getGlobalPose();

    PxVec3 v(dx, dy, dz);
    //v.normalize();
    transform.rotate(v);

    m_rigidActor->setGlobalPose(transform, true);
}

// ========================================================================= //

const Ogre::Vector3 PhysicsComponent::getPosition(void) const
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    return Ogre::Vector3(transform.p.x, 
                         transform.p.y, 
                         transform.p.z);
}

// ========================================================================= //

const Ogre::Quaternion PhysicsComponent::getOrientation(void) const
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    return Ogre::Quaternion(transform.q.w,
                            transform.q.x,
                            transform.q.y,
                            transform.q.z);
}

// ========================================================================= //

void PhysicsComponent::setMaterial(World& world,
                                   const PxReal staticFriction,
                                   const PxReal dynamicFriction,
                                   const PxReal restitution)
{
    m_mat = world.getPScene()->getSDK()->createMaterial(staticFriction,
                                                        dynamicFriction,
                                                        restitution);
}

// ========================================================================= //


