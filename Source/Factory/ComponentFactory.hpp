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
// File: ComponentFactory.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ComponentFactory class.
// ========================================================================= //

#ifndef __COMPONENTFACTORY_HPP__
#define __COMPONENTFACTORY_HPP__

// ========================================================================= //

#include "Component/Component.hpp"

// ========================================================================= //
// Contains pools of Components, creates Component objects.
class ComponentFactory final
{
public:
    // Default initializes pools.
    explicit ComponentFactory(void);

    // Empty destructor.
    ~ComponentFactory(void);

    // Allocates component pools.
    void init(void);

    // Creation functions:
    ActorComponentPtr createActorComponent(void);
    CameraComponentPtr createCameraComponent(void);
    LightComponentPtr createLightComponent(void);
    ModelComponentPtr createModelComponent(void);
    PhysicsComponentPtr createPhysicsComponent(void);
    SceneComponentPtr createSceneComponent(void);

private:
    // Component pools. The idea here is to avoid dynamic allocation of 
    // any Components during the game state. They are allocated during the
    // World's intialization and retrieved from a memory pool as needed.
    std::shared_ptr<Pool<ActorComponent>> m_actorComponentPool;
    std::shared_ptr<Pool<CameraComponent>> m_cameraComponentPool;
    std::shared_ptr<Pool<LightComponent>> m_lightComponentPool;
    std::shared_ptr<Pool<ModelComponent>> m_modelComponentPool;
    std::shared_ptr<Pool<PhysicsComponent>> m_physicsComponentPool;
    std::shared_ptr<Pool<SceneComponent>> m_sceneComponentPool;
};

// ========================================================================= //

#endif

// ========================================================================= //