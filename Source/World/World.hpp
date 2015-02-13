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

#include "stdafx.hpp"

// ========================================================================= //

class ActorComponent;
class CameraComponent;
class Entity;
class EntityPool;
class Input;
class LightComponent;
class ModelComponent;
class Physics;
class PhysicsComponent;
template<typename T> class Pool;
class PScene;
class SceneComponent;

typedef ActorComponent* ActorComponentPtr;
typedef CameraComponent* CameraComponentPtr;
typedef Entity* EntityPtr;
typedef LightComponent* LightComponentPtr;
typedef ModelComponent* ModelComponentPtr;
typedef PhysicsComponent* PhysicsComponentPtr;
typedef SceneComponent* SceneComponentPtr;

// ========================================================================= //
// Represents everything in the physical game world. The World holds 
// collections of Systems (which have collections of Components) and Entities
// (which have pointers to the needed Components in the Systems).
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
    void init(void);

    // De-allocates all data.
    void destroy(void);

    // Entity pool functions:

    // Get the next free Entity from the internal pool and return a pointer.
    EntityPtr createEntity(void);

    // Calls destroy on Entity and 
    void destroyEntity(EntityPtr);

    // Checks if each Entity's Components have been initialized. Returns true
    // if they all have. Should be called after initial scene setup.
    const bool checkEntities(void) const;

    // Updates every active Entity in the game world.
    void update(void);

    // === //

    // Ogre3D scene functions:

    // Sets direction of the world's directional light.
    void setLightDirection(const Ogre::Real, const Ogre::Real, const Ogre::Real);

    // Sets colour of world's directional light.
    void setLightColour(const Ogre::Real, const Ogre::Real, const Ogre::Real);

    // Sets colour of world's ambient light.
    void setAmbientLight(const Ogre::Real, const Ogre::Real, const Ogre::Real);

    // Sets fog mode, colour, along with any parameters needed.
    void setFog(const Ogre::FogMode mode, 
                const Ogre::Real r, 
                const Ogre::Real g, 
                const Ogre::Real b,
                const Ogre::Real expDensity,
                const Ogre::Real linearStart,
                const Ogre::Real linearEnd);

    // === //

    // Component factory functions:

    ActorComponentPtr createActorComponent(void);
    CameraComponentPtr createCameraComponent(void);
    LightComponentPtr createLightComponent(void);
    ModelComponentPtr createModelComponent(void);
    PhysicsComponentPtr createPhysicsComponent(void);
    SceneComponentPtr createSceneComponent(void);

    // === //

    // Getters:

    // Returns pointer to Ogre::SceneManager for this world.
    Ogre::SceneManager* getSceneManager(void) const;

    // Returns pointer to the Ogre::Viewport for this world.
    Ogre::Viewport* getViewport(void) const;

    // Returns pointer to PScene (physics scene).
    std::shared_ptr<PScene> getPScene(void) const;

    // Returns reference to internal Input instance.
    Input* getInput(void) const;

    // === // 

    struct Dependencies{
        Ogre::Root* root;
        Ogre::Viewport* viewport;
        std::shared_ptr<Physics> physics;
        Input* input;
    };

private:
    // Ogre3D.
    Ogre::Root* m_root;
    Ogre::SceneManager* m_scene;
    Ogre::Viewport*    m_viewport;
    Ogre::Light* m_dLight; // Directional light.

    // PhysX.
    std::shared_ptr<Physics> m_physics;
    std::shared_ptr<PScene> m_PScene;

    // Entity pool.
    std::shared_ptr<EntityPool> m_entityPool;

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

inline void World::injectDependencies(const Dependencies& deps){
    m_root = deps.root;
    m_viewport = deps.viewport;
    m_physics = deps.physics;
    m_input = deps.input;
}

// Ogre3D scene functions:

inline void World::setLightDirection(const Ogre::Real x,
                                     const Ogre::Real y,
                                     const Ogre::Real z){
    m_dLight->setDirection(x, y, z);
}

inline void World::setLightColour(const Ogre::Real r,
                                  const Ogre::Real g,
                                  const Ogre::Real b){
    m_dLight->setDiffuseColour(r, g, b);
    m_dLight->setSpecularColour(r, g, b);
}

inline void World::setAmbientLight(const Ogre::Real r,
                                   const Ogre::Real g,
                                   const Ogre::Real b){
    m_scene->setAmbientLight(Ogre::ColourValue(r, g, b));
}

inline void World::setFog(const Ogre::FogMode mode,
                          const Ogre::Real r,
                          const Ogre::Real g,
                          const Ogre::Real b,
                          const Ogre::Real expDensity,
                          const Ogre::Real linearStart = 50.f,
                          const Ogre::Real linearEnd = 500.f){
    m_scene->setFog(mode, 
                    Ogre::ColourValue(r, g, b),
                    expDensity,
                    linearStart,
                    linearEnd);
}

// Getters:

inline Ogre::SceneManager* World::getSceneManager(void) const{
    return m_scene;
}

inline Ogre::Viewport* World::getViewport(void) const{
    return m_viewport;
}

inline std::shared_ptr<PScene> World::getPScene(void) const{
    return m_PScene;
}

inline Input* World::getInput(void) const{
    return m_input;
}

// ========================================================================= //

#endif

// ========================================================================= //