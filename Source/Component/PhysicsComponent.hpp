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

	// Empty.
	virtual void init(EntityPtr, World&) override;

	// Initializes PhysX actor, adds to World's PxScene.
	virtual void init(World&, 
					  const Type,
					  PxGeometry&,
					  const PxReal staticFriction = 0.5f, 
					  const PxReal dynamicFriction = 0.5f, 
					  const PxReal restitution = 0.3f,
					  const PxReal density = 10.f);

	// Removes PhysX actor from World's PxScene.
	virtual void destroy(EntityPtr, World&) override;

	// Retrieves the actor's position and orientation, applies them to the
	// attached SceneComponent for rendering.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

	// Component functions:

	// Sets PhysX pose position to these coordinates.
	void setPosition(const PxReal, const PxReal, const PxReal);

	// Sets PhysX pose orientation to this quaternion.
	void setOrientation(const PxReal, const PxReal, const PxReal, 
						const PxReal);

	// Translates the PhysX pose directly.
	void translate(const PxReal, const PxReal, const PxReal);

	// Rotates the PhysX pose by this vector.
	void rotate(const PxReal, const PxReal, const PxReal);

	// Getters:

	PxRigidActor* getRigidActor(void) const;
	PxRigidStatic* getStaticActor(void) const;
	PxRigidDynamic* getDynamicActor(void) const;

private:
	union{
		PxRigidStatic* m_sActor;
		PxRigidDynamic* m_dActor;
	};

	PxRigidActor* m_actor;
	PxMaterial* m_mat;
};

// ========================================================================= //

inline PxRigidActor* PhysicsComponent::getRigidActor(void) const{
	return m_actor;
}

inline PxRigidStatic* PhysicsComponent::getStaticActor(void) const{
	return m_sActor;
}

inline PxRigidDynamic* PhysicsComponent::getDynamicActor(void) const{
	return m_dActor;
}

// ========================================================================= //

#endif

// ========================================================================= //