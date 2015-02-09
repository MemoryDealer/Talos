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


	if (type == Type::STATIC){
		m_sActor = PxCreateStatic(*world.getPScene()->getSDK(),
								  PxTransform(PxVec3(0.f, 0.f, 0.f)),
								  geometry,
								  *m_mat);

		world.getPScene()->getScene()->addActor(*m_sActor);
		m_actor = m_sActor;
	}
	else if(type == Type::DYNAMIC){
		m_dActor = PxCreateDynamic(*world.getPScene()->getSDK(),
								   PxTransform(PxVec3(0.f, 0.f, 0.f)),
								   geometry,
								   *m_mat,
								   density);

		world.getPScene()->getScene()->addActor(*m_dActor);
		m_actor = m_dActor;
	}
}

// ========================================================================= //

// Empty.
void PhysicsComponent::destroy(EntityPtr, World&)
{

}

// ========================================================================= //

// Empty.
void PhysicsComponent::update(EntityPtr entity, World& world)
{
	PxTransform transform = m_actor->getGlobalPose();

	SceneComponentPtr sceneC = static_cast<SceneComponentPtr>
		(entity->getComponentPtr("SceneComponent"));

	sceneC->getSceneNode()->setPosition(transform.p.x, transform.p.y, transform.p.z);
}

// ========================================================================= //

// Empty.
void PhysicsComponent::message(const ComponentMessage&)
{

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

