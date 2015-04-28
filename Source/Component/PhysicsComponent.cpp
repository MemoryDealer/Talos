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
#include "Physics/Cooker.hpp"
#include "Physics/PScene.hpp"
#include "PhysicsComponent.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

PhysicsComponent::PhysicsComponent(void) :
m_rigidActor(nullptr),
m_kinematicActors(),
m_type(Type::Box),
m_mat(nullptr),
m_density(1.f),
m_kinematic(false)
{
    
}

// ========================================================================= //

PhysicsComponent::~PhysicsComponent(void)
{

}

// ========================================================================= //

void PhysicsComponent::init(void)
{
    
}

// ========================================================================= //

void PhysicsComponent::init(EntityPtr entity)
{
    // If no custom material has been assigned, use the default.
    if (!m_mat){
        m_mat = this->getWorld()->getPScene()->getDefaultMaterial();
    }

    // Add all attach meshes for multi-model.
    if (entity->hasComponent<MultiModelComponent>()){
        auto rootNode = entity->getComponent<SceneComponent>()->getSceneNode();
        auto itr = rootNode->getChildIterator();
        while (itr.hasMoreElements()){
            auto node = static_cast<Ogre::SceneNode*>(itr.getNext());
            auto aoitr = node->getAttachedObjectIterator();
            while (aoitr.hasMoreElements()){
                auto ao = aoitr.getNext();
                if (typeid(*ao) == typeid(Ogre::Entity)){
                    auto e = static_cast<Ogre::Entity*>(ao);
                    this->createActor(e,
                                      entity->getID(),
                                      node->_getDerivedPosition(),
                                      node);
                }
            }
        }
    }
    // Just add the one mesh.
    else if(entity->hasComponent<ModelComponent>()){
        auto e = entity->getComponent<ModelComponent>()->getOgreEntity();
        auto pos = entity->getComponent<SceneComponent>()->getPosition();
        this->createActor(e, entity->getID(), pos);
    }        
}

// ========================================================================= //

void PhysicsComponent::createActor(Ogre::Entity* e, 
                                   const EntityID id,
                                   const Ogre::Vector3& p,
                                   Ogre::SceneNode* node)
{
    PxVec3 pos(Physics::toPx(p));
    std::shared_ptr<PxGeometry> geometry;
    PxRigidDynamic* rigidActor = nullptr;

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

            // Create dynamic actor.
            rigidActor = PxCreateDynamic(*this->getWorld()->getPScene()->getSDK(),
                                           PxTransform(pos),
                                           *geometry,
                                           *m_mat,
                                           m_density);
        }
        break;

    case Type::Sphere:
        {
            // Create sphere geometry.
            geometry.reset(new PxSphereGeometry(e->getBoundingRadius()));
            //Assert(geometry->isValid(), "Invalid PxSphereGeometry for PhysicsComponent");

            // Create dynamic actor.
            rigidActor = PxCreateDynamic(*this->getWorld()->getPScene()->getSDK(),
                                           PxTransform(pos),
                                           *geometry,
                                           *m_mat,
                                           m_density);
        }
        break;

    case Type::TriangleMesh:
        {
            PxTriangleMesh* mesh = this->getWorld()->getPScene()->
                getCooker()->createTriangleMesh(e->getMesh());
            
            geometry.reset(new PxTriangleMeshGeometry(mesh));

            /*m_rigidActor = PxCreateDynamic(*this->getWorld()->getPScene()->getSDK(),
                                           PxTransform(pos),
                                           *geometry,
                                           *m_mat,
                                           m_density);*/
            rigidActor = PxCreateKinematic(*this->getWorld()->getPScene()->getSDK(),
                                             PxTransform(pos),
                                             *geometry,
                                             *m_mat,
                                             m_density);
            //Assert(static_cast<PxTriangleMeshGeometry*>(geometry.get())->isValid(), "Invalid geom for PhysicsComponent");
        }
        break;

    case Type::ConvexMesh:
        {
            PxConvexMesh* mesh = this->getWorld()->getPScene()->
                getCooker()->createConvexMesh(e->getMesh());

            geometry.reset(new PxConvexMeshGeometry(mesh));
            rigidActor = PxCreateDynamic(*this->getWorld()->getPScene()->getSDK(),
                                           PxTransform(pos),
                                           *geometry,
                                           *m_mat,
                                           m_density);                        
        }
        break;
    }

    // Assign actor's user data to EntityID.
    rigidActor->userData = reinterpret_cast<void*>(
        static_cast<const EntityID>(id));

    // Set this actor to kinematic if specified.
    if (m_kinematic){
        rigidActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // Add to kinematic actor list.
        KinematicActor k;
        k.node = node;
        k.actor = rigidActor;
        m_kinematicActors.push_back(k);
    }

    // Add actor to PhysX scene.
    this->getWorld()->getPScene()->getScene()->addActor(*rigidActor);

    // Add to debug drawer if activated.
    if (this->getWorld()->getPScene()->isUsingDebugDrawer()){
        this->getWorld()->getPScene()->addToDebugDrawer(rigidActor, *geometry);
    }

    // Assign rigid actor pointer.  
    m_rigidActor = rigidActor;
}

// ========================================================================= //

void PhysicsComponent::destroy(void)
{    
    if (m_rigidActor){
        this->getWorld()->getPScene()->getScene()->removeActor(*m_rigidActor);
    }

    for (auto& i : m_kinematicActors){
        this->getWorld()->getPScene()->getScene()->removeActor(*i.actor);
    }
}

// ========================================================================= //

void PhysicsComponent::update(void)
{
    
}

// ========================================================================= //

void PhysicsComponent::message(ComponentMessage& msg)
{

}

// ========================================================================= //

// Component functions:

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

void PhysicsComponent::rotate(const PxReal rx,
                              const PxReal ry,
                              const PxReal rz)
{
    // @TODO: This doesn't worK!
    //PxTransform transform = m_rigidActor->getGlobalPose();

    //PxVec3 v(rx, ry, rz);
    ////v.normalize();
    //transform.rotate(v);

    //m_rigidActor->setGlobalPose(transform, true);
}

// ========================================================================= //

void PhysicsComponent::updateAllKinematics(void)
{
    for (auto& i : m_kinematicActors){
        PxTransform transform = i.actor->getGlobalPose();

        transform.p = Physics::toPx(i.node->_getDerivedPosition());
        transform.q = Physics::toPx(i.node->_getDerivedOrientation());

        i.actor->setGlobalPose(transform);
    }
}

// ========================================================================= //

// Getters:

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

PxRigidDynamic* PhysicsComponent::getRigidActor(void) const
{
    return m_rigidActor;
}

// ========================================================================= //

const bool PhysicsComponent::isKinematic(void) const
{
    return m_kinematic;
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void PhysicsComponent::setPosition(const Ogre::Vector3& pos)
{
    /*for (auto& i : m_rigidActors){
        PxTransform transform = i->getGlobalPose();

        transform.p = Physics::toPx(pos);
        i->setGlobalPose(transform, true);
    }*/

    PxTransform transform = m_rigidActor->getGlobalPose();

    transform.p = Physics::toPx(pos);
    m_rigidActor->setGlobalPose(transform, true);
}

// ========================================================================= //

void PhysicsComponent::setPosition(const PxReal x,
                                   const PxReal y,
                                   const PxReal z)
{
    this->setPosition(Ogre::Vector3(x, y, z));
}

// ========================================================================= //

void PhysicsComponent::setOrientation(const Ogre::Quaternion& orientation)
{
    PxTransform transform = m_rigidActor->getGlobalPose();

    transform.q = Physics::toPx(orientation);
    m_rigidActor->setGlobalPose(transform, true);
}

// ========================================================================= //

void PhysicsComponent::setOrientation(const PxReal w,
                                      const PxReal x,
                                      const PxReal y,
                                      const PxReal z)
{
    this->setOrientation(Ogre::Quaternion(w, x, y, z));
}

// ========================================================================= //

void PhysicsComponent::setType(const Type& type)
{
    m_type = type;
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

void PhysicsComponent::setDensity(const PxReal density)
{
    m_density = density;
}

// ========================================================================= //

void PhysicsComponent::setKinematic(const bool kinematic)
{
    m_kinematic = kinematic;
}

// ========================================================================= //


