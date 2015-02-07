// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Physics.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Physics class.
// ========================================================================= //

#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //
// Holds top-level PhysX objects for initialization of the physics engine.
class Physics final
{
	friend class PScene;

public:
	explicit Physics(void);

	~Physics(void);

	const bool init(void);

	void destroy(void);

private:
	PxFoundation* m_foundation;
	PxPhysics* m_physx;
	PxDefaultAllocator m_defaultAllocator;
	PxDefaultErrorCallback m_defaultErrorCallback;
};

// ========================================================================= //

#endif

// ========================================================================= //