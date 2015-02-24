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
// File: Entity.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Entity class.
// ========================================================================= //

#include "Component/ActorComponent.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/ComponentMessage.hpp"
#include "Component/LightComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity.hpp"

// ========================================================================= //

Entity::Entity(void) :
m_components(),
m_componentsLinked(false),
m_id(0),
m_next(nullptr)
{
    
}

// ========================================================================= //

Entity::~Entity(void)
{

}

// ========================================================================= //

void Entity::init(World& world)
{
    
}

// ========================================================================= //

void Entity::destroy(World& world)
{
    for (auto& itr : m_components){
        itr.second->destroy(this, world);
    }
}

// ========================================================================= //

void Entity::update(World& world)
{
    for (auto& itr : m_components){
        itr.second->update(this, world);
    }
}

// ========================================================================= //

ComponentPtr Entity::attachComponent(const ComponentPtr component)
{
    Assert(component->getType() > 0 &&
           component->getType() < Component::Type::NumTypes,
           "Invalid component type added to entity");

    // Insert new ComponentPtr into hash table.
    m_components[&typeid(*component)] = component;

    // Set this Entity's linked flag to false.
    m_componentsLinked = false;
    return component;
}

// ========================================================================= //

void Entity::detachComponent(const ComponentPtr component)
{
    m_components.erase(&typeid(*component));
    m_components[&typeid(*component)] = nullptr;
}

// ========================================================================= //

void Entity::detachComponent(const int type)
{
    Assert(type > 0 && type < Component::Type::NumTypes,
           "Invalid component type removed from entity");

    
}

// ========================================================================= //

void Entity::linkComponents(void)
{
    // Attach any needed components to the scene component.
    SceneComponentPtr sceneC = nullptr;
    if ((sceneC = this->getComponent<SceneComponent>()) != nullptr){
        // Attach components to the scene component.
        // Test for model component.
        ModelComponentPtr modelC = this->getComponent<ModelComponent>();
        if (modelC){
            sceneC->attachModel(modelC);
        }

        // Test for light component.
        LightComponentPtr lightC = this->getComponent<LightComponent>();
        if (lightC){
            sceneC->attachLight(lightC);
        }
    }

    // Attach any needed components to the actor component.
    ActorComponentPtr actorC = nullptr;
    if ((actorC = this->getComponent<ActorComponent>()) != nullptr){
        // Test for camera component.
        CameraComponentPtr cameraC = this->getComponent<CameraComponent>();
        if (cameraC){
            actorC->attachCamera(cameraC);
        }

        // Test for model component.
        ModelComponentPtr modelC = this->getComponent<ModelComponent>();
        if (modelC){
            actorC->attachModel(modelC);
        }

        // Test for light component.
        LightComponentPtr lightC = this->getComponent<LightComponent>();
        if (lightC){
            actorC->attachLight(lightC);
        }
    }

    m_componentsLinked = true;
}

// ========================================================================= //

const bool Entity::checkComponents(void)
{
    if (!m_componentsLinked){
        // Link all attached components.
        this->linkComponents();
    }

    for (auto& itr : m_components){
        if (itr.second->isInitialized() == false){
            return false;
        }
    }

    return true;
}

// ========================================================================= //

void Entity::message(const ComponentMessage& msg)
{
    Assert(msg.type != ComponentMessageType::NIL, "NIL Entity message!");

    // Broadcast to all attached components.
    for (auto& itr : m_components){
        itr.second->message(msg);
    }
}

// ========================================================================= //