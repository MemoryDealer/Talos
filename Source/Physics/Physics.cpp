// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Physics.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Physics class.
// ========================================================================= //

#include "Physics.hpp"

// ========================================================================= //

Physics::Physics(void) :
m_foundation(nullptr),
m_physx(nullptr),
m_defaultAllocator(),
m_defaultErrorCallback()
{
	
}

// ========================================================================= //

Physics::~Physics(void)
{

}

// ========================================================================= //

const bool Physics::init(void)
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION,
									  m_defaultAllocator,
									  m_defaultErrorCallback);

	bool recordMemoryAllocations = true;
	m_physx = PxCreatePhysics(PX_PHYSICS_VERSION,
								*m_foundation,
								PxTolerancesScale(),
								recordMemoryAllocations,
								nullptr);
	if (m_physx == nullptr){
		return false;
	}

	return true;
}

// ========================================================================= //

void Physics::destroy(void)
{
	m_physx->release();
	m_foundation->release();
}

// ========================================================================= //