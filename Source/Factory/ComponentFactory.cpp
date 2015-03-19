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
// File: ComponentFactory.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements ComponentFactory class.
// ========================================================================= //

#include "ComponentFactory.hpp"
#include "Pool/Pool.hpp"

// ========================================================================= //

ComponentFactory::ComponentFactory(void) :
m_actorComponentPool(nullptr),
m_cameraComponentPool(nullptr),
m_collisionComponentPool(nullptr),
m_lightComponentPool(nullptr),
m_modelComponentPool(nullptr),
m_networkComponentPool(nullptr),
m_physicsComponentPool(nullptr),
m_rotationComponentPool(nullptr),
m_sceneComponentPool(nullptr)
{

}

// ========================================================================= //

ComponentFactory::~ComponentFactory(void)
{
    // Pools automatically deallocated.
}

// ========================================================================= //

void ComponentFactory::init(void)
{
    // Allocate Component pools.
    // @TODO: Read sizes from config file.
    m_actorComponentPool.reset(new Pool<ActorComponent>(256));
    m_cameraComponentPool.reset(new Pool<CameraComponent>(5));
    m_collisionComponentPool.reset(new Pool<CollisionComponent>(1024));
    m_lightComponentPool.reset(new Pool<LightComponent>(16));
    m_modelComponentPool.reset(new Pool<ModelComponent>(1024));
    m_networkComponentPool.reset(new Pool<NetworkComponent>(256));
    m_physicsComponentPool.reset(new Pool<PhysicsComponent>(1024));
    m_rotationComponentPool.reset(new Pool<RotationComponent>(1024));
    m_sceneComponentPool.reset(new Pool<SceneComponent>(1024));
}

// ========================================================================= //

// Creation functions:

// ========================================================================= //

ActorComponentPtr ComponentFactory::createActorComponent(void)
{
    return m_actorComponentPool->create();
}

// ========================================================================= //

CameraComponentPtr ComponentFactory::createCameraComponent(void)
{
    return m_cameraComponentPool->create();
}

// ========================================================================= //

CollisionComponentPtr ComponentFactory::createCollisionComponent(void)
{
    return m_collisionComponentPool->create();
}

// ========================================================================= //

LightComponentPtr ComponentFactory::createLightComponent(void)
{
    return m_lightComponentPool->create();
}

// ========================================================================= //

ModelComponentPtr ComponentFactory::createModelComponent(void)
{
    return m_modelComponentPool->create();
}

// ========================================================================= //

NetworkComponentPtr ComponentFactory::createNetworkComponent(void)
{
    return m_networkComponentPool->create();
}

// ========================================================================= //

PhysicsComponentPtr ComponentFactory::createPhysicsComponent(void)
{
    return m_physicsComponentPool->create();
}

// ========================================================================= //

RotationComponentPtr ComponentFactory::createRotationComponent(void)
{
    return m_rotationComponentPool->create();
}

// ========================================================================= //

SceneComponentPtr ComponentFactory::createSceneComponent(void)
{
    return m_sceneComponentPool->create();
}

// ========================================================================= //