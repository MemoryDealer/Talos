// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PhysicsComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements StaticPhysicsComponent class.
// ========================================================================= //

#include "Entity/Entity.hpp"
#include "Physics/PScene.hpp"
#include "PhysicsComponent.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

PhysicsComponent::PhysicsComponent(void) :
Component(),
m_sActor(nullptr),
m_dActor(nullptr),
m_actor(nullptr),
m_mat(nullptr)
{

}

// ========================================================================= //

PhysicsComponent::~PhysicsComponent(void)
{

}

// ========================================================================= //

void PhysicsComponent::init(EntityPtr, World&)
{

}

// ========================================================================= //

void PhysicsComponent::init(World& world, 
							const Type type,
							PxGeometry& geometry,
							const PxReal staticFriction,
							const PxReal dynamicFriction,
							const PxReal restitution,
							const PxReal density)
{
	// Create the physics material.
	m_mat = world.getPScene()->getSDK()->createMaterial(staticFriction,
														dynamicFriction,
														restitution);

	// Create a static PhysX actor.
	if (type == Type::STATIC){
		m_sActor = PxCreateStatic(*world.getPScene()->getSDK(),
								  PxTransform(PxVec3(0.f, 0.f, 0.f)),
								  geometry,
								  *m_mat);

		world.getPScene()->getScene()->addActor(*m_sActor);
		m_actor = m_sActor;
	}
	// Create a dynamic PhysX actor.
	else if(type == Type::DYNAMIC){
		m_dActor = PxCreateDynamic(*world.getPScene()->getSDK(),
								   PxTransform(PxVec3(0.f, 0.f, 0.f)),
								   geometry,
								   *m_mat,
								   density);
		
		world.getPScene()->getScene()->addActor(*m_dActor);
		m_actor = m_dActor;
	}

	// @TODO: Set user data of actor to EntityPtr?

	/*PxRigidStatic* p = PxCreatePlane(*m_world.getPScene()->m_physx,
	PxPlane(PxVec3(0.f, 1.f, 0.f), 50.f),
	*mat);*/
}

// ========================================================================= //

void PhysicsComponent::destroy(EntityPtr entity, World& world)
{
	Assert(m_actor != nullptr, "Null m_actor!");

	world.getPScene()->getScene()->removeActor(*m_actor);
}

// ========================================================================= //

void PhysicsComponent::update(EntityPtr entity, World& world)
{
	PxTransform transform = m_actor->getGlobalPose();

	SceneComponentPtr sceneC = static_cast<SceneComponentPtr>
		(entity->getComponentPtr("SceneComponent"));

	Assert(sceneC != nullptr, 
		   "PhysicsComponent without corresponding SceneComponent");

	// Set position and orientation.
	sceneC->setPosition(transform.p.x, transform.p.y, transform.p.z);
	sceneC->setOrientation(transform.q.w, transform.q.x, transform.q.y,
						   transform.q.z);
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
	PxTransform transform = m_actor->getGlobalPose();

	transform.p.x = x;
	transform.p.y = y;
	transform.p.z = z;

	m_actor->setGlobalPose(transform, true);
}

// ========================================================================= //

// Sets PhysX pose orientation to this quaternion.
void PhysicsComponent::setOrientation(const PxReal w, 
									  const PxReal x, 
									  const PxReal y,
									  const PxReal z)
{
	PxTransform transform = m_actor->getGlobalPose();

	transform.q.w = w;
	transform.q.x = x;
	transform.q.y = y;
	transform.q.z = z;

	m_actor->setGlobalPose(transform, true);
}

// ========================================================================= //

void PhysicsComponent::translate(const PxReal dx, 
								 const PxReal dy, 
								 const PxReal dz)
{
	PxTransform transform = m_actor->getGlobalPose();

	transform.p.x += dx;
	transform.p.y += dy;
	transform.p.z += dz;

	m_actor->setGlobalPose(transform, true);
}

// ========================================================================= //

void PhysicsComponent::rotate(const PxReal dx,
							  const PxReal dy,
							  const PxReal dz)
{
	// @TODO: This doesn't worK!
	PxTransform transform = m_actor->getGlobalPose();

	PxVec3 v(dx, dy, dz);
	//v.normalize();
	transform.rotate(v);

	m_actor->setGlobalPose(transform, true);
}

// ========================================================================= //

