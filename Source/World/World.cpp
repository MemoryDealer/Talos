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
// File: World.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements World class.
// ========================================================================= //

#include "Component/ActorComponent.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/EntityPool.hpp"
#include "Environment.hpp"
#include "Physics/PScene.hpp"
#include "Pool/Pool.hpp"
#include "World.hpp"

// ========================================================================= //

World::World(void) :
m_root(nullptr),
m_scene(nullptr),
m_viewport(nullptr),
m_environment(nullptr),
m_graphics(),
m_physics(nullptr),
m_PScene(nullptr),
m_entityPool(nullptr),
m_player(nullptr),
m_actorComponentPool(nullptr),
m_cameraComponentPool(nullptr),
m_modelComponentPool(nullptr),
m_sceneComponentPool(nullptr),
m_input(nullptr)
{
    
}

// ========================================================================= //

World::~World(void)
{

}

// ========================================================================= //

void World::init(void)
{
    // Create Ogre scene for rendering.
    m_scene = m_root->createSceneManager(Ogre::ST_GENERIC);

    // Initialize environment.
    m_environment.reset(new Environment(this, m_graphics));
    m_environment->init();
    
    // Create physics scene.
    m_PScene.reset(new PScene(m_physics));
    m_PScene->init();

    // Allocate Entity pool.
    // @TODO: Read pool size from config file.
    m_entityPool.reset(new EntityPool(256));

    // Allocate Component pools.
    // @TODO: Read sizes from config file.
    m_actorComponentPool.reset(new Pool<ActorComponent>(256));
    m_cameraComponentPool.reset(new Pool<CameraComponent>(5));
    m_lightComponentPool.reset(new Pool<LightComponent>(16));
    m_modelComponentPool.reset(new Pool<ModelComponent>(1024));
    m_physicsComponentPool.reset(new Pool<PhysicsComponent>(1024));
    m_sceneComponentPool.reset(new Pool<SceneComponent>(1024));
}

// ========================================================================= //

void World::destroy(void)
{
    m_root->destroySceneManager(m_scene);
}

// ========================================================================= //

EntityPtr World::createEntity(void)
{
    return m_entityPool->create();
}

// ========================================================================= //

void World::destroyEntity(EntityPtr e)
{
    return m_entityPool->destroy(e);
}

// ========================================================================= //

const bool World::checkEntities(void) const
{
    for (int i = 0; i < m_entityPool->m_poolSize; ++i){
        EntityPtr entity = &m_entityPool->m_pool[i];
        if (entity->checkComponents() == false){
            return false;
        }
    }

    return true;
}

// ========================================================================= //

void World::update(void)
{
    // Simulate physics first.
   

    for (int i = 0; i < m_entityPool->m_poolSize; ++i){
        m_entityPool->m_pool[i].update(*this); // Dereference self.
    }

    m_PScene->simulate();

    m_environment->update();
}

// ========================================================================= //

// Component factory functions:

ActorComponentPtr World::createActorComponent(void){
    return m_actorComponentPool->create();
}

// ========================================================================= //

CameraComponentPtr World::createCameraComponent(void){
    return m_cameraComponentPool->create();
}

// ========================================================================= //

LightComponentPtr World::createLightComponent(void){
    return m_lightComponentPool->create();
}

// ========================================================================= //

ModelComponentPtr World::createModelComponent(void){
    return m_modelComponentPool->create();
}

// ========================================================================= //

PhysicsComponentPtr World::createPhysicsComponent(void){
    return m_physicsComponentPool->create();
}

// ========================================================================= //

SceneComponentPtr World::createSceneComponent(void){
    return m_sceneComponentPool->create();
}

// ========================================================================= //