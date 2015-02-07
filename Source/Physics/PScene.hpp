// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PhysicsScene.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines PhysicsScene class.
// ========================================================================= //

#ifndef __PSCENE_HPP__
#define __PSCENE_HPP__

// ========================================================================= //

#include "Physics.hpp"

// ========================================================================= //
// A single physics scene to be used by a state. PScene is easier to type.
class PScene final
{
public:
	explicit PScene(const std::shared_ptr<Physics>);

	~PScene(void);

	const bool init(void);

	void destroy(void);

	void simulate(PxReal);

public:
	PxPhysics* m_physx;
	PxScene* m_scene;
	PxDefaultCpuDispatcher* m_cpuDispatcher;
};

// ========================================================================= //

#endif

// ========================================================================= //