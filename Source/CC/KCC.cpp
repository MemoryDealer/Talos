// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: KCC.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements KCC class.
// ========================================================================= //

#include "KCC.hpp"
#include "Physics/PScene.hpp"
#include "World/World.hpp"

// ========================================================================= //

KCC::KCC(void) :
m_controller(nullptr)
{

}

// ========================================================================= //

KCC::~KCC(void)
{

}

// ========================================================================= //

bool KCC::init(World& world)
{
	PxCapsuleControllerDesc desc;
	desc.position = PxExtendedVec3(0.f, 0.f, 0.f);
	desc.height = 1.8288f;
	desc.radius = 0.3048f;

	PxMaterial* mat = world.getPScene()->getSDK()->createMaterial(0.5f,
																  0.5f,
																  0.1f);
	desc.material = mat;

	m_controller = world.getPScene()->getControllerManager()->
		createController(desc);

	if (m_controller == nullptr){
		return false;
	}

	return true;
}

// ========================================================================= //

void KCC::destroy(World& world)
{
	// ?
}

// ========================================================================= //

PxExtendedVec3 KCC::update(const PxReal dx, const PxReal dy, const PxReal dz)
{
	const PxReal gravity = -9.81f / 16.f; // Magic random value.
	PxVec3 disp(dx, gravity, dz);

	m_controller->move(disp, 0.001f, 16.f, 0);
	
	// Get updated position.
	PxExtendedVec3 pos = m_controller->getPosition();
	// Adjust for head height.
	pos.y += 0.8644f;
	return pos;
}

// ========================================================================= //