// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
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

    // Raycasting:

    struct Ray{
        explicit Ray(void) :
        origin(PxVec3(0.f, 0.f, 0.f)),
        dir(PxVec3(0.f, -1.f, 0.f)),
        dist(100.f),
        hit(),
        flags(PxQueryFlag::eANY_HIT) { }

        PxVec3 origin;
        PxVec3 dir;
        PxReal dist;
        PxRaycastBuffer hit;
        PxQueryFlags flags;
    };

    bool raycast(Ray& ray);

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