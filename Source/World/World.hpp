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
// File: World.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines World class.
// ========================================================================= //

#ifndef __WORLD_HPP__
#define __WORLD_HPP__

// ========================================================================= //

#include "Component/ComponentDecls.hpp"
#include "stdafx.hpp"

// ========================================================================= //

class Client;
class EntityPool;
class Environment;
class Input;
class Physics;
template<typename T> class Pool;
class PScene;
class Server;
//
//typedef std::unordered_map<const std::type_info*, 
//    std::shared_ptr<Pool<T>>> PoolHashTable;

// ========================================================================= //
// Represents everything in the physical game world. The World holds a
// collection of Entities (which have pointers to the needed Components).
// Entities and Components are retrieved from pools in the World allocated
// during World::init().
class World final
{
public:
    // Default initializes member data.
    explicit World(void);

    // Empty destructor.
    ~World(void);

    // Passes needed dependencies into World.
    struct Dependencies;
    void injectDependencies(const Dependencies&);

    // Allocates the Ogre::SceneManager.
    void init(const bool usePhysics = false);

    // De-allocates all data.
    void destroy(void);

    // Resumes world state, assigns main camera to viewport.
    void resume(void);

    // Entity pool functions:

    // Get the next free Entity from the internal pool and return a pointer.
    EntityPtr createEntity(void);

    // Calls destroy on Entity.
    void destroyEntity(EntityPtr);

    // Checks if each Entity's Components have been initialized and linked. 
    // Returns true if they all have. Should be called after initial scene 
    // setup.
    const bool checkEntities(void) const;

    // Updates every active Entity in the game world, updates environment.
    void update(void);

    // === //

    // Physics functions:

    // Creates physics scene (PScene).
    void initPhysics(void);

    // === //
    
    // Network functions:

    // Initializes Server, update will process Server updates.
    void initServer(void);

    // Destroys Server.
    void destroyServer(void);

    // Initializes Client, update will process Client updates.
    void initClient(void);

    // Destroys Client.
    void destroyClient(void);

    // === //

    // Component factory functions:

    ActorComponentPtr createActorComponent(void);
    CameraComponentPtr createCameraComponent(void);
    LightComponentPtr createLightComponent(void);
    ModelComponentPtr createModelComponent(void);
    PhysicsComponentPtr createPhysicsComponent(void);
    SceneComponentPtr createSceneComponent(void);

    // Creates component from pool, initializes it, and attaches it 
    // to the entity.
    template<typename T>
    void attachComponent(EntityPtr entity);

    // === //

    // Getters:

    // Returns pointer to Ogre::Root object.
    Ogre::Root* getRoot(void) const;

    // Returns pointer to Ogre::SceneManager for this world.
    Ogre::SceneManager* getSceneManager(void) const;

    // Returns pointer to the Ogre::Viewport for this world.
    Ogre::Viewport* getViewport(void) const;

    // Returns pointer to internal Environment.
    std::shared_ptr<Environment> getEnvironment(void) const;

    // Returns graphics settings data.
    const Graphics& getGraphics(void) const;

    // Returns pointer to player-controlled Entity.
    EntityPtr getPlayer(void) const;

    // Returns pointer to camera which views the game world.
    CameraComponentPtr getMainCamera(void) const;

    // Returns pointer to PScene (physics scene).
    std::shared_ptr<PScene> getPScene(void) const;

    // Returns pointer to Server.
    std::shared_ptr<Server> getServer(void) const;

    // Returns pointer to Client.
    std::shared_ptr<Client> getClient(void) const;

    // Returns reference to internal Input instance.
    Input* getInput(void) const;

    // Setters:

    // Sets pointer to Entity controlled by player.
    void setPlayer(const EntityPtr);

    // Sets pointer to main camera which views the game world.
    void setMainCamera(CameraComponentPtr);

    // === // 

    struct Dependencies{
        Ogre::Root* root;
        Ogre::Viewport* viewport;
        std::shared_ptr<Physics> physics;
        Input* input;
        Graphics graphics;
        std::shared_ptr<Server> server;
        std::shared_ptr<Client> client;
    };

private:
    // Ogre3D.
    Ogre::Root* m_root;
    Ogre::SceneManager* m_scene;
    Ogre::Viewport* m_viewport;
    
    // Environment control.
    std::shared_ptr<Environment> m_environment;

    // Graphics settings.
    Graphics m_graphics;

    // PhysX.    
    std::shared_ptr<Physics> m_physics;
    std::shared_ptr<PScene> m_PScene;
    bool m_usePhysics;

    // Network.
    std::shared_ptr<Server> m_server;
    std::shared_ptr<Client> m_client;
    bool m_useServer;
    bool m_useClient;

    // Entity pool.
    std::shared_ptr<EntityPool> m_entityPool;

    // The Entity the player controls.
    EntityPtr m_player;
    bool m_hasPlayer;

    // Main camera which renders to viewport.
    CameraComponentPtr m_mainCameraC;

    // Component pools. The idea here is to avoid dynamic allocation of 
    // any Components during the game state. They are allocated during the
    // World's intialization and retrieved from a memory pool as needed.
    std::shared_ptr<Pool<ActorComponent>> m_actorComponentPool;
    std::shared_ptr<Pool<CameraComponent>> m_cameraComponentPool;
    std::shared_ptr<Pool<LightComponent>> m_lightComponentPool;
    std::shared_ptr<Pool<ModelComponent>> m_modelComponentPool;
    std::shared_ptr<Pool<PhysicsComponent>> m_physicsComponentPool;
    std::shared_ptr<Pool<SceneComponent>> m_sceneComponentPool;

    // Input component.
    Input* m_input;
};

// ========================================================================= //

// Getters:

inline Ogre::Root* World::getRoot(void) const{
    return m_root;
}

inline Ogre::SceneManager* World::getSceneManager(void) const{
    return m_scene;
}

inline Ogre::Viewport* World::getViewport(void) const{
    return m_viewport;
}

inline std::shared_ptr<Environment> World::getEnvironment(void) const{
    return m_environment;
}

inline const Graphics& World::getGraphics(void) const{
    return m_graphics;
}

inline EntityPtr World::getPlayer(void) const{
    return m_player;
}

inline CameraComponentPtr World::getMainCamera(void) const{
    return m_mainCameraC;
}

inline std::shared_ptr<PScene> World::getPScene(void) const{
    return m_PScene;
}

inline std::shared_ptr<Server> World::getServer(void) const{
    return m_server;
}

inline std::shared_ptr<Client> World::getClient(void) const{
    return m_client;
}

inline Input* World::getInput(void) const{
    return m_input;
}

// Setters:

inline void World::setMainCamera(CameraComponentPtr cameraC){
    m_mainCameraC = cameraC;
}

// ========================================================================= //

#endif

// ========================================================================= //