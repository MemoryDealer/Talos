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
#include "Component/LightComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/EntityPool.hpp"
#include "Environment.hpp"
#include "Factory/ComponentFactory.hpp"
#include "Network/Client/Client.hpp"
#include "Network/Server/Server.hpp"
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
m_usePhysics(false),
m_network(nullptr),
m_server(nullptr),
m_client(nullptr),
m_entityPool(nullptr),
m_componentFactory(nullptr),
m_player(nullptr),
m_hasPlayer(false),
m_mainCameraC(nullptr),
m_input(nullptr)
{
    
}

// ========================================================================= //

World::~World(void)
{

}

// ========================================================================= //

void World::injectDependencies(const Dependencies& deps)
{
    m_root = deps.root;
    m_viewport = deps.viewport;
    m_physics = deps.physics;
    m_input = deps.input;
    m_graphics = deps.graphics;
    m_server = deps.server;
    m_client = deps.client;
}

// ========================================================================= //

void World::init(const bool usePhysics)
{
    // Create Ogre scene for rendering.
    m_scene = m_root->createSceneManager(Ogre::ST_GENERIC);

    // Initialize environment.
    m_environment.reset(new Environment(this, m_graphics));
    m_environment->init();
    
    if (usePhysics == true){
        this->initPhysics();
    }

    // Allocate Entity pool.
    // @TODO: Read pool size from config file.
    m_entityPool.reset(new EntityPool(256));

    // Allocate component factory (component pools).
    m_componentFactory.reset(new ComponentFactory());
    m_componentFactory->init();

    // Assign Network pointer if server or client is active.
    if (m_server->initialized() == true){
        m_network = m_server.get();
    }
    else if (m_client->initialized() == true){
        m_network = m_client.get();
    }
}

// ========================================================================= //

void World::destroy(void)
{
    m_environment->destroy();

    for (int i = 0; i < m_entityPool->m_poolSize; ++i){
        m_entityPool->m_pool[i].destroy(*this);
    }

    if (m_usePhysics){
        m_PScene->destroy();
    }

    m_scene->destroyAllCameras();
    m_scene->clearScene();
    m_root->destroySceneManager(m_scene);
}

// ========================================================================= //

void World::pause(void)
{
    m_environment->pause();
}

// ========================================================================= //

void World::resume(void)
{
    m_viewport->setCamera(m_mainCameraC->getCamera());

    m_environment->resume();

    // Detach Network pointer if network services are disabled.
    if (m_network != nullptr){
        if (m_network->initialized() == false){
            m_network = nullptr;
        }
    }
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
    // Network.
    if (m_network != nullptr){
        m_network->update();
    }

    for (int i = 0; i < m_entityPool->m_poolSize; ++i){
        m_entityPool->m_pool[i].update(*this); // Dereference self.
    }

    if (m_usePhysics){
        m_PScene->simulate();
    }

    m_environment->update();
}

// ========================================================================= //

// Physics functions:

// ========================================================================= //

void World::initPhysics(void)
{
    m_PScene.reset(new PScene(m_physics));
    m_PScene->init();
    m_usePhysics = true;
}

// ========================================================================= //

// Network functions:

// ========================================================================= //

void World::initServer(const int port, const std::string& username)
{
    Assert(m_network == nullptr, 
           "Trying to init Server with Network already initialized");

    m_network = m_server.get();
    m_network->init(port, username);
}

// ========================================================================= //

void World::destroyServer(void)
{
    m_network->destroy();
    m_network = nullptr;
}

// ========================================================================= //

void World::initClient(void)
{
    Assert(m_network == nullptr,
           "Trying to init Client with Network already initialized");

    m_network = m_client.get();
    m_network->init();
}

// ========================================================================= //

void World::destroyClient(void)
{
    m_network->destroy();
    m_network = nullptr;
}

// ========================================================================= //

template<typename T>
typename World::componentReturn<T>::type World::attachComponent(EntityPtr entity)
{ 
    return nullptr;
}

// ========================================================================= //

// Called by all World::attachComponent<T> specializations.
static void initComponent(ComponentPtr component, 
                          EntityPtr entity, 
                          World* world)
{
    component->init(*world);
    entity->attachComponent(component);
}

// ========================================================================= //

// Template specializations for attachComponent():

// ========================================================================= //

template<> struct World::componentReturn<ActorComponent>{ 
    typedef ActorComponentPtr type; 
};

template<> World::componentReturn<ActorComponent>::type World::attachComponent<ActorComponent>(EntityPtr entity)
{
    ActorComponentPtr c = m_componentFactory->createActorComponent();
    initComponent(c, entity, this);
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<CameraComponent>{ 
    typedef CameraComponentPtr type; 
};

template<> World::componentReturn<CameraComponent>::type World::attachComponent<CameraComponent>(EntityPtr entity)
{
    CameraComponentPtr c = m_componentFactory->createCameraComponent();
    initComponent(c, entity, this);
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<LightComponent>{ 
    typedef LightComponentPtr type; 
};

template<> World::componentReturn<LightComponent>::type World::attachComponent<LightComponent>(EntityPtr entity)
{
    LightComponentPtr c = m_componentFactory->createLightComponent();
    initComponent(c, entity, this);
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<ModelComponent>{ 
        typedef ModelComponentPtr type; 
};

template<> World::componentReturn<ModelComponent>::type World::attachComponent<ModelComponent>(EntityPtr entity)
{
    ModelComponentPtr c = m_componentFactory->createModelComponent();
    initComponent(c, entity, this);
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<PhysicsComponent>{ 
    typedef PhysicsComponentPtr type; 
};

template<> World::componentReturn<PhysicsComponent>::type World::attachComponent<PhysicsComponent>(EntityPtr entity)
{
    PhysicsComponentPtr c = m_componentFactory->createPhysicsComponent();
    initComponent(c, entity, this);
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<SceneComponent>{ 
    typedef SceneComponentPtr type; 
};

template<> World::componentReturn<SceneComponent>::type World::attachComponent<SceneComponent>(EntityPtr entity)
{
    SceneComponentPtr c = m_componentFactory->createSceneComponent();
    initComponent(c, entity, this);
    return c;
}

// ========================================================================= //

void World::setPlayer(const EntityPtr player)
{
    m_player = player;
    m_mainCameraC = m_player->getComponent<CameraComponent>();
    m_hasPlayer = true;
}

// ========================================================================= //