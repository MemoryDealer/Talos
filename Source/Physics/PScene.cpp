// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PhysicsScene.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements PhysicsScene class.
// ========================================================================= //

#include "PScene.hpp"

// ========================================================================= //

PScene::PScene(const std::shared_ptr<Physics> physics) :
m_physx(physics->m_physx),
m_scene(nullptr),
m_cpuDispatcher(nullptr)
{
	
}

// ========================================================================= //

PScene::~PScene(void)
{

}

// ========================================================================= //

const bool PScene::init(void)
{
	PxSceneDesc desc(m_physx->getTolerancesScale());
	desc.gravity = PxVec3(0.f, -9.81f, 0.f);

	if (!desc.cpuDispatcher){
		m_cpuDispatcher = PxDefaultCpuDispatcherCreate(4);
		if (!m_cpuDispatcher){
			return false;
		}
		desc.cpuDispatcher = m_cpuDispatcher;
	}

	if (!desc.filterShader){
		desc.filterShader = PxDefaultSimulationFilterShader;
	}

#ifdef PX_WINDOWS
	// ...
#endif

	m_scene = m_physx->createScene(desc);
	if (!m_scene){
		return false;
	}

	return true;
}

// ========================================================================= //

void PScene::destroy(void)
{

}

// ========================================================================= //

void PScene::simulate(PxReal speed)
{
	const PxReal step = 1.f / 16.f;

	m_scene->simulate(step * speed);
	m_scene->fetchResults(true);
}

// ========================================================================= //