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

#include "Component/AllComponents.hpp"
#include "Entity/EntityPool.hpp"
#include "Environment.hpp"
#include "Input/Input.hpp"
#include "Network/NullNetwork.hpp"
#include "Network/Client/Client.hpp"
#include "Network/Server/Server.hpp"
#include "Physics/PScene.hpp"
#include "Pool/Pool.hpp"
#include "Rendering/Listeners/WeaponListener.hpp"
#include "System/System.hpp"
#include "System/SystemManager.hpp"
#include "World.hpp"

// ========================================================================= //

static Network* SNullNetwork = new NullNetwork();

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
m_entityIDMap(),
m_componentPools(),
m_systemManager(nullptr),
m_player(nullptr),
m_hasPlayer(false),
m_mainCameraC(nullptr),
m_input(nullptr),
m_soundEngine(nullptr),
m_sounds(),
m_musicCount(0)
{
    m_sounds.reserve(10);
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
    m_soundEngine = deps.soundEngine;
}

// ========================================================================= //

void World::init(const bool usePhysics)
{
    // Create Ogre scene for rendering.
    m_scene = m_root->createSceneManager(Ogre::ST_GENERIC);
    m_scene->addRenderQueueListener(new WeaponRenderListener());

    switch (m_graphics.shadows){
    default:
    case Graphics::Off:
        m_scene->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_NONE);
        break;

    case Graphics::High:
        m_scene->setShadowTextureSelfShadow(true);

        // Set shadow shader.
        m_scene->setShadowTextureCasterMaterial("Sparks/shadow_caster");

        // Setup shadow texture settings.
        m_scene->setShadowTextureCount(8);
        m_scene->setShadowTextureSize(1024);

        // Using float16 for R and G channels.
        m_scene->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_RGB);

        // VSM doesn't need biasing, this would be worthless.
        m_scene->setShadowCasterRenderBackFaces(false);

        // Use integrated additive shadows with these shaders.
        m_scene->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);

        m_scene->setShadowTextureFSAA(16);

        /*const uint32_t rtts = m_scene->getShadowTextureCount();
        for (uint32_t i = 0; i < rtts; ++i){
        Ogre::TexturePtr tex = m_scene->getShadowTexture(i);
        Ogre::Viewport* vp =
        tex->getBuffer()->getRenderTarget()->getViewport(0);
        vp->setBackgroundColour(Ogre::ColourValue(1.f, 1.f, 1.f, 1.f));
        vp->setClearEveryFrame(true);
        }*/
        break;
    }

    // Initialize environment.
    m_environment.reset(new Environment(shared_from_this(), m_graphics));
    m_environment->init();

    if (usePhysics){
        this->initPhysics();
    }

    // Allocate Entity pool.
    // @TODO: Read pool size from config file.
    m_entityPool.reset(new EntityPool(256));

    // Allocate component pools.
    const uint32_t common = 1024;
    m_componentPools[&typeid(ActorComponent)].reset(
        new Pool<ActorComponent>(common));
    m_componentPools[&typeid(CameraComponent)].reset(
        new Pool<CameraComponent>(5));
    m_componentPools[&typeid(CollisionComponent)].reset(
        new Pool<CollisionComponent>(common));
    m_componentPools[&typeid(LightComponent)].reset(
        new Pool<LightComponent>(16));
    m_componentPools[&typeid(LinkComponent)].reset(
        new Pool<LinkComponent>(common));
    m_componentPools[&typeid(ModelComponent)].reset(
        new Pool<ModelComponent>(common));
    m_componentPools[&typeid(MultiModelComponent)].reset(
        new Pool<MultiModelComponent>(common));
    m_componentPools[&typeid(NetworkComponent)].reset(
        new Pool<NetworkComponent>(4));
    m_componentPools[&typeid(ParticleComponent)].reset(
        new Pool<ParticleComponent>(common));
    m_componentPools[&typeid(PhysicsComponent)].reset(
        new Pool<PhysicsComponent>(common));
    m_componentPools[&typeid(RotationComponent)].reset(
        new Pool<RotationComponent>(common));
    m_componentPools[&typeid(SceneComponent)].reset(
        new Pool<SceneComponent>(common));
    m_componentPools[&typeid(SoundComponent)].reset(
        new Pool<SoundComponent>(32));
    m_componentPools[&typeid(StatComponent)].reset(
        new Pool<StatComponent>(common));
    m_componentPools[&typeid(TrackComponent)].reset(
        new Pool<TrackComponent>(common));
    m_componentPools[&typeid(WeaponComponent)].reset(
        new Pool<WeaponComponent>(common));

    m_systemManager.reset(new SystemManager(shared_from_this()));

    // Assign Network pointer if server or client is active.
    if (m_server->initialized()){
        m_network = m_server.get();
    }
    else if (m_client->initialized()){
        m_network = m_client.get();
    }
    else{
        m_network = SNullNetwork;
    }
}

// ========================================================================= //

void World::destroy(void)
{
    m_environment->destroy();

    for (int i = 0; i < m_entityPool->m_poolSize; ++i){
        m_entityPool->m_pool[i].destroy();
    }
    m_entityIDMap.clear();

    if (m_usePhysics){
        m_PScene->destroy();
    }

    // Stop all sounds.
    m_soundEngine->stopAllSounds();
    for (std::vector<irrklang::ISound*>::iterator itr = std::begin(m_sounds);
         itr != std::end(m_sounds);
         ++itr){
        (*itr)->drop();
    }
    m_sounds.clear();
    m_musicCount = 0;

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

    // Assign Network pointer to NullNetwork if network services are disabled.
    if (m_network){
        if (!m_network->initialized()){
            m_network = SNullNetwork;
        }
    }
}

// ========================================================================= //

void World::update(void)
{
    // Update each world component.

    m_network->update();

    m_systemManager->update();

    for (int i = 0; i < m_entityPool->m_poolSize; ++i){
        if (m_player != nullptr){
            if (m_entityPool->m_pool[i].getID() == m_player->getID()){
                continue;
            }
        }
        m_entityPool->m_pool[i].update();
    }

    if (m_usePhysics){
        m_PScene->simulate();
    }

    m_environment->update();

    // Update listener position in 3D audio engine.
    if (m_player){
        Ogre::Vector3 pos = m_player->getComponent<ActorComponent>()->
            getPosition();
        Ogre::Vector3 look = m_player->getComponent<ActorComponent>()->
            getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;

        m_soundEngine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z),
                                           irrklang::vec3df(-look.x, look.y, -look.z)); 
    }
}

// ========================================================================= //

// Entity functions:

// ========================================================================= //

EntityPtr World::createEntity(void)
{
    EntityPtr e = m_entityPool->create();
    m_entityIDMap[e->getID()] = e;
    return e;
}

// ========================================================================= //

void World::destroyEntity(EntityPtr e)
{
    m_entityIDMap.erase(e->getID());
    return m_entityPool->destroy(e);
}

// ========================================================================= //

const bool World::setupEntities(void) const
{
    for (int i = 0; i < m_entityPool->m_poolSize; ++i){
        EntityPtr entity = &m_entityPool->m_pool[i];
        m_systemManager->processEntity(entity);
    }

    return true;
}

// ========================================================================= //

EntityPtr World::getEntityPtr(const EntityID id)
{
    if (m_entityIDMap.count(id) != 0){
        return m_entityIDMap[id];
    }

    return nullptr;
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
    Assert(m_network->initialized() == false, 
           "Trying to init Server with Network already initialized");

    m_network = m_server.get();
    m_network->init(port, username);
}

// ========================================================================= //

void World::destroyServer(void)
{
    m_network->destroy();
    m_network = SNullNetwork;
}

// ========================================================================= //

void World::initClient(void)
{
    Assert(m_network->initialized() == false,
           "Trying to init Client with Network already initialized");

    m_network = m_client.get();
    m_network->init();
}

// ========================================================================= //

void World::destroyClient(void)
{
    m_network->destroy();
    m_network = SNullNetwork;
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
                          std::shared_ptr<World> world)
{
    component->setWorld(world);
    component->init();
    entity->attachComponent(component);
}

// ========================================================================= //

// Template specializations for attachComponent():

// ========================================================================= //

template<> struct World::componentReturn<ActorComponent>{ 
    typedef ActorComponentPtr type; 
};

template<> World::componentReturn<ActorComponent>::type 
World::attachComponent<ActorComponent>(EntityPtr entity)
{
    Pool<ActorComponent>* pool = static_cast<Pool<ActorComponent>*>(
        m_componentPools[&typeid(ActorComponent)].get());

    ActorComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<CameraComponent>{ 
    typedef CameraComponentPtr type; 
};

template<> World::componentReturn<CameraComponent>::type 
World::attachComponent<CameraComponent>(EntityPtr entity)
{
    Pool<CameraComponent>* pool = static_cast<Pool<CameraComponent>*>(
        m_componentPools[&typeid(CameraComponent)].get());

    CameraComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<CollisionComponent>{
    typedef CollisionComponentPtr type;
};

template<> World::componentReturn<CollisionComponent>::type 
World::attachComponent<CollisionComponent>(EntityPtr entity)
{
    Pool<CollisionComponent>* pool = static_cast<Pool<CollisionComponent>*>(
        m_componentPools[&typeid(CollisionComponent)].get());

    CollisionComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<LightComponent>{ 
    typedef LightComponentPtr type;
};

template<> World::componentReturn<LightComponent>::type 
World::attachComponent<LightComponent>(EntityPtr entity)
{
    Pool<LightComponent>* pool = static_cast<Pool<LightComponent>*>(
        m_componentPools[&typeid(LightComponent)].get());

    LightComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<LinkComponent>{
    typedef LinkComponentPtr type;
};

template<> World::componentReturn<LinkComponent>::type
World::attachComponent<LinkComponent>(EntityPtr entity)
{
    Pool<LinkComponent>* pool = static_cast<Pool<LinkComponent>*>(
        m_componentPools[&typeid(LinkComponent)].get());

    LinkComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<ModelComponent>{ 
    typedef ModelComponentPtr type; 
};

template<> World::componentReturn<ModelComponent>::type 
World::attachComponent<ModelComponent>(EntityPtr entity)
{
    Pool<ModelComponent>* pool = static_cast<Pool<ModelComponent>*>(
        m_componentPools[&typeid(ModelComponent)].get());

    ModelComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<MultiModelComponent>{
    typedef MultiModelComponentPtr type;
};

template<> World::componentReturn<MultiModelComponent>::type
World::attachComponent<MultiModelComponent>(EntityPtr entity)
{
    Pool<MultiModelComponent>* pool = static_cast<Pool<MultiModelComponent>*>(
        m_componentPools[&typeid(MultiModelComponent)].get());

    MultiModelComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<NetworkComponent>{
    typedef NetworkComponentPtr type;
};

template<> World::componentReturn<NetworkComponent>::type
World::attachComponent<NetworkComponent>(EntityPtr entity)
{
    Pool<NetworkComponent>* pool = static_cast<Pool<NetworkComponent>*>(
        m_componentPools[&typeid(NetworkComponent)].get());

    NetworkComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<ParticleComponent>{
    typedef ParticleComponentPtr type;
};

template<> World::componentReturn<ParticleComponent>::type
World::attachComponent<ParticleComponent>(EntityPtr entity)
{
    Pool<ParticleComponent>* pool = static_cast<Pool<ParticleComponent>*>(
        m_componentPools[&typeid(ParticleComponent)].get());

    ParticleComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<PhysicsComponent>{ 
    typedef PhysicsComponentPtr type; 
};

template<> World::componentReturn<PhysicsComponent>::type 
World::attachComponent<PhysicsComponent>(EntityPtr entity)
{
    Pool<PhysicsComponent>* pool = static_cast<Pool<PhysicsComponent>*>(
        m_componentPools[&typeid(PhysicsComponent)].get());

    PhysicsComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<RotationComponent>{
    typedef RotationComponentPtr type;
};

template<> World::componentReturn<RotationComponent>::type
World::attachComponent<RotationComponent>(EntityPtr entity)
{
    Pool<RotationComponent>* pool = static_cast<Pool<RotationComponent>*>(
        m_componentPools[&typeid(RotationComponent)].get());

    RotationComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<SceneComponent>{ 
    typedef SceneComponentPtr type; 
};

template<> World::componentReturn<SceneComponent>::type 
World::attachComponent<SceneComponent>(EntityPtr entity)
{
    Pool<SceneComponent>* pool = static_cast<Pool<SceneComponent>*>(
        m_componentPools[&typeid(SceneComponent)].get());

    SceneComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<SoundComponent>{
    typedef SoundComponentPtr type;
};

template<> World::componentReturn<SoundComponent>::type
World::attachComponent<SoundComponent>(EntityPtr entity)
{
    Pool<SoundComponent>* pool = static_cast<Pool<SoundComponent>*>(
        m_componentPools[&typeid(SoundComponent)].get());

    SoundComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<StatComponent>{
    typedef StatComponentPtr type;
};

template<> World::componentReturn<StatComponent>::type
World::attachComponent<StatComponent>(EntityPtr entity)
{
    Pool<StatComponent>* pool = static_cast<Pool<StatComponent>*>(
        m_componentPools[&typeid(StatComponent)].get());

    StatComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<TrackComponent>{
    typedef TrackComponentPtr type;
};

template<> World::componentReturn<TrackComponent>::type 
World::attachComponent<TrackComponent>(EntityPtr entity)
{
    Pool<TrackComponent>* pool = static_cast<Pool<TrackComponent>*>(
        m_componentPools[&typeid(TrackComponent)].get());

    TrackComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

template<> struct World::componentReturn<WeaponComponent>{
    typedef WeaponComponentPtr type;
};

template<> World::componentReturn<WeaponComponent>::type
World::attachComponent<WeaponComponent>(EntityPtr entity)
{
    Pool<WeaponComponent>* pool = static_cast<Pool<WeaponComponent>*>(
        m_componentPools[&typeid(WeaponComponent)].get());

    WeaponComponentPtr c = pool->create();
    initComponent(c, entity, shared_from_this());
    return c;
}

// ========================================================================= //

void World::addSystem(System* system)
{
    m_systemManager->addSystem(system);
}

// ========================================================================= //

void World::addEntityToSystem(EntityPtr entity)
{
    m_systemManager->processEntity(entity);
}

// ========================================================================= //

CommandPtr World::handleInput(const SDL_Event& e)
{
    m_input->handle(e);
    
    return nullptr;
}

// ========================================================================= //

void World::playMusic(const std::string& file, const uint32_t instance)
{
    m_sounds.push_back(m_soundEngine->play2D(file.c_str(), true));
    ++m_musicCount;
}

// ========================================================================= //

void World::stopMusic(const uint32_t instance)
{
    m_sounds[instance]->stop();
    m_sounds[instance]->drop();
    m_sounds.erase(m_sounds.begin() + instance);
    --m_musicCount;
}

// ========================================================================= //

void World::setPlayer(const EntityPtr player)
{
    m_player = player;
    m_mainCameraC = m_player->getComponent<CameraComponent>();
    m_hasPlayer = true;

    if (m_network->getLocalPlayer()){
        m_network->getLocalPlayer()->entity = player;
    }
}

// ========================================================================= //