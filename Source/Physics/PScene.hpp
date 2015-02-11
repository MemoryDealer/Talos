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

class PDebugDrawer;

// ========================================================================= //
// A single physics scene to be used by a state (PScene is easier to type).
class PScene final
{
public:
	explicit PScene(const std::shared_ptr<Physics>);

	~PScene(void);

	const bool init(void);

	void destroy(void);

	void simulate(PxReal speed = 1.0f);

	// Allocates internal debug drawer.
	void loadDebugDrawer(void);

	// Adds the actor to the internal debug drawer.
	void addToDebugDrawer(PxRigidActor*, PxGeometry&);

	// Getters:

	PxPhysics* getSDK(void) const;

	PxScene* getScene(void) const;

	PxMaterial* getDefaultMaterial(void) const;

	PxControllerManager* getControllerManager(void) const;

	// Returns true if the debug drawer is activated.
	const bool isUsingDebugDrawer(void) const;

	// Setters:

public:
	PxPhysics* m_physx;
	PxScene* m_scene;
	PxMaterial* m_defaultMaterial;
	PxDefaultCpuDispatcher* m_cpuDispatcher;
	PxControllerManager* m_controllerManager;
	std::shared_ptr<PDebugDrawer> m_debugDrawer;
	bool m_useDebugDrawer;
};

// ========================================================================= //

// Getters:

inline PxPhysics* PScene::getSDK(void) const{
	return m_physx;
}

inline PxScene* PScene::getScene(void) const{
	return m_scene;
}

inline PxMaterial* PScene::getDefaultMaterial(void) const{
	return m_defaultMaterial;
}

inline PxControllerManager* PScene::getControllerManager(void) const{
	return m_controllerManager;
}

inline const bool PScene::isUsingDebugDrawer(void) const{
	return m_useDebugDrawer;
}

// ========================================================================= //

#endif

// ========================================================================= //