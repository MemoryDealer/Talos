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

	void simulate(PxReal speed = 1.0f);

	// Getters:

	PxPhysics* getSDK(void) const;

	PxScene* getScene(void) const;

	PxControllerManager* getControllerManager(void) const;

public:
	PxPhysics* m_physx;
	PxScene* m_scene;
	PxDefaultCpuDispatcher* m_cpuDispatcher;
	PxControllerManager* m_controllerManager;
};

// ========================================================================= //

// Getters:

inline PxPhysics* PScene::getSDK(void) const{
	return m_physx;
}

inline PxScene* PScene::getScene(void) const{
	return m_scene;
}

inline PxControllerManager* PScene::getControllerManager(void) const{
	return m_controllerManager;
}

// ========================================================================= //

#endif

// ========================================================================= //