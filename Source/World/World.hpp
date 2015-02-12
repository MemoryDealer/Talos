// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
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
	Ogre::Viewport*	m_viewport;
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