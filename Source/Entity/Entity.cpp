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
m_componentRegistry(),
m_activeComponents(),
m_componentsLinked(false),
m_id(0),
m_next(nullptr)
{
    // Allocate array of total possible components.
    m_componentRegistry.resize(Component::Type::NumTypes);
    std::fill(m_componentRegistry.begin(), m_componentRegistry.end(), nullptr);
}

// ========================================================================= //

Entity::~Entity(void)
{

}

// ========================================================================= //

void Entity::init(World& world)
{
    for (auto itr = m_activeComponents.begin();
         itr != m_activeComponents.end();
         ++itr){
        (*itr)->init(this, world);
    }
}

// ========================================================================= //

void Entity::destroy(World& world)
{
    for (auto itr = m_activeComponents.begin();
         itr != m_activeComponents.end();
         ++itr){
        (*itr)->destroy(this, world);
    }
}

// ========================================================================= //

void Entity::update(World& world)
{
    for (auto itr = m_activeComponents.begin();
         itr != m_activeComponents.end();
         ++itr){
        (*itr)->update(this, world);
    }
}

// ========================================================================= //

ComponentPtr Entity::attachComponent(const ComponentPtr component)
{
    Assert(component->getType() > 0 &&
           component->getType() < Component::Type::NumTypes,
           "Invalid component type added to entity");

    // Add to active component list.
    m_activeComponents.push_back(component);

    // Add Component to its registry slot.
    m_componentRegistry[component->getType()] = component;

    // Set this Entity's linked flag to false.
    m_componentsLinked = false;
    return component;
}

// ========================================================================= //

void Entity::detachComponent(const ComponentPtr component)
{
    for (auto itr = m_activeComponents.begin();
         itr != m_activeComponents.end();){
        if (*itr == component){
            itr = m_activeComponents.erase(itr);
        }
        else{
            ++itr;
        }
    }

    // Remove link from component registry.
    m_componentRegistry[component->getType()] = nullptr;
}

// ========================================================================= //

void Entity::detachComponent(const int type)
{
    Assert(type > 0 && type < Component::Type::NumTypes,
           "Invalid component type removed from entity");

    for (auto itr = m_activeComponents.begin();
         itr != m_activeComponents.end();){
        if ((*itr)->getType() == type){
            itr = m_activeComponents.erase(itr);
        }
        else{
            ++itr;
        }
    }

    m_componentRegistry[type] = nullptr;
}

// ========================================================================= //

void Entity::linkComponents(void)
{
    // Attach any needed components to the scene component.
    SceneComponentPtr sceneC = nullptr;
    if ((sceneC = getSceneComponent()) != nullptr){
        // Attach components to the scene component.
        // Test for model component.
        ModelComponentPtr modelC = this->getModelComponent();
        if (modelC){
            sceneC->attachModel(modelC);
        }

        // Test for light component.
        LightComponentPtr lightC = this->getLightComponent();
        if (lightC){
            sceneC->attachLight(lightC);
        }
    }

    // Attach any needed components to the actor component.
    ActorComponentPtr actorC = nullptr;
    if ((actorC = this->getActorComponent()) != nullptr){
        // Test for camera component.
        CameraComponentPtr cameraC = this->getCameraComponent();
        if (cameraC){
            actorC->attachCamera(cameraC);
        }

        // Test for model component.
        ModelComponentPtr modelC = this->getModelComponent();
        if (modelC){
            actorC->attachModel(modelC);
        }

        // Test for light component.
        LightComponentPtr lightC = this->getLightComponent();
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

    for (auto itr = m_activeComponents.begin();
         itr != m_activeComponents.end();
         ++itr){
        if ((*itr)->isInitialized() == false){
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
    for (auto itr = m_activeComponents.begin();
         itr != m_activeComponents.end();
         ++itr){
        (*itr)->message(msg);
    }
}

// ========================================================================= //

ActorComponentPtr Entity::getActorComponent(void) const
{
    return static_cast<ActorComponentPtr>(
        m_componentRegistry[Component::Type::Actor]);
}

// ========================================================================= //

CameraComponentPtr Entity::getCameraComponent(void) const
{
    return static_cast<CameraComponentPtr>(
        m_componentRegistry[Component::Type::Camera]);
}

// ========================================================================= //

LightComponentPtr Entity::getLightComponent(void) const
{
    return static_cast<LightComponentPtr>(
        m_componentRegistry[Component::Type::Light]);
}

// ========================================================================= //

ModelComponentPtr Entity::getModelComponent(void) const
{
    return static_cast<ModelComponentPtr>(
        m_componentRegistry[Component::Type::Model]);
}

// ========================================================================= //

PhysicsComponentPtr Entity::getPhysicsComponent(void) const
{
    return static_cast<PhysicsComponentPtr>(
        m_componentRegistry[Component::Type::Physics]);
}

// ========================================================================= //

SceneComponentPtr Entity::getSceneComponent(void) const
{
    return static_cast<SceneComponentPtr>(
        m_componentRegistry[Component::Type::Scene]);
}

// ========================================================================= //