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
// File: SystemManager.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements SystemManager class.
// ========================================================================= //

#include "CollisionSystem.hpp"
#include "Component/ActorComponent.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/LightComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/NetworkComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/Entity.hpp"
#include "PhysicsSystem.hpp"
#include "System.hpp"
#include "SystemManager.hpp"

// ========================================================================= //

SystemManager::SystemManager(World* world) :
m_systems(),
m_world(world)
{

}

// ========================================================================= //

SystemManager::~SystemManager(void)
{
   
}

// ========================================================================= //

void SystemManager::destroy(void)
{
    for (auto& i : m_systems){
        delete i.second;
    }

    m_systems.clear();
}

// ========================================================================= //

void SystemManager::addSystem(System* system)
{
    m_systems[&typeid(*system)] = system;
    system->setWorld(m_world);
}

// ========================================================================= //

void SystemManager::processEntity(EntityPtr entity)
{
    // Collision System.
    if (this->hasSystem<CollisionSystem>()){
        if (entity->hasComponent<SceneComponent>() &&
            entity->hasComponent<CollisionComponent>()){
            this->getSystem<CollisionSystem>()->attachEntity(entity);
        }
    }
    // Physics System.
    if (this->hasSystem<PhysicsSystem>()){
        if (entity->hasComponent<SceneComponent>() &&
            entity->hasComponent<PhysicsComponent>()){
            this->getSystem<PhysicsSystem>()->attachEntity(entity);
        }
    }    

    // Additional setup, linking component data together.
    if (entity->hasComponent<ActorComponent>() ||
        entity->hasComponent<SceneComponent>()){
        SceneComponentPtr sceneC = (entity->hasComponent<ActorComponent>()) ?
            entity->getComponent<ActorComponent>() :
            entity->getComponent<SceneComponent>();

        // Link camera to scene component.
        if (entity->hasComponent<CameraComponent>()){
            sceneC->attachCamera(
                entity->getComponent<CameraComponent>()->getCamera());
        }

        // Link light to scene component.
        if (entity->hasComponent<LightComponent>()){
            sceneC->getSceneNode()->attachObject(
                entity->getComponent<LightComponent>()->getLight());
        }

        // Link mesh to scene component.
        if (entity->hasComponent<ModelComponent>()){
            sceneC->getSceneNode()->attachObject(
                entity->getComponent<ModelComponent>()->getOgreEntity());
        }

        // Link actor to its associated network component.
        if (entity->hasComponent<NetworkComponent>()){
            // Set the world pointer (needed for client input sequence number).
            entity->getComponent<NetworkComponent>()->setWorld(m_world);

            entity->getComponent<NetworkComponent>()->
                setActorComponentPtr(
                static_cast<ActorComponentPtr>(sceneC));
        }
    }
    
}

// ========================================================================= //

void SystemManager::update(void)
{
    for (auto& i : m_systems){
        i.second->update();
    }
}

// ========================================================================= //
