// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PhysicsComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines PhysicsComponent class.
// ========================================================================= //

#ifndef __PHYSICSCOMPONENT_HPP__
#define __PHYSICSCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //

class PhysicsComponent : public Component
{
public:
	// Default initializes member data, stores mesh filename.
	explicit PhysicsComponent(void);

	// Empty destructor.
	virtual ~PhysicsComponent(void) override;

	enum Type{
		STATIC = 0,
		DYNAMIC
	};

	// 
	virtual void init(EntityPtr, World&) override;
	virtual void init(World&, 
					  const Type,
					  PxGeometry&,
					  const PxReal staticFriction = 0.5f, 
					  const PxReal dynamicFriction = 0.5f, 
					  const PxReal restitution = 0.1f,
					  const PxReal density = 1.f);

	// Empty.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

	// Component functions:

	// Translates the PhysX pose directly.
	void translate(const PxReal, const PxReal, const PxReal);

private:
	union{
		PxRigidStatic* m_sActor;
		PxRigidDynamic* m_dActor;
	};

	PxRigidActor* m_actor;
	PxMaterial* m_mat;
};

// ========================================================================= //

#endif

// ========================================================================= //