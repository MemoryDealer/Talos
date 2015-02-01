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
#include "Component/Component.hpp"
#include "Component/ComponentPool.hpp"

// ========================================================================= //

class EntityPool;

// ========================================================================= //
// Represents everything in the physical game world. The World holds 
// collections of Systems (which have collections of Components) and Entities
// (which have pointers to the needed Components in the Systems).
class World
{
public:
	// Default initializes member data.
	explicit World(void);

	// Empty destructor.
	~World(void);

	// Allocates the Ogre::SceneManager.
	void init(Ogre::Root*, Ogre::Viewport*);

	// De-allocates all data.
	void destroy(Ogre::Root*);

	// Entity pool functions:

	// Get the next free Entity from the internal pool and return a pointer.
	EntityPtr createEntity(void);

	// Calls destroy on Entity and 
	void destroyEntity(EntityPtr);

	// Updates every active Entity in the game world.
	void update(void);

	// Component factory functions.
	CameraComponentPtr createCameraComponent(void);
	FirstPersonComponentPtr createFirstPersonComponent(void);
	InputComponentPtr createInputComponent(void);
	ModelComponentPtr createModelComponent(void);
	SceneComponentPtr createSceneComponent(void);

	// Getters:

	// Returns pointer to Ogre::SceneManager for this world.
	Ogre::SceneManager* getSceneManager(void) const;

	// Returns pointer to the Ogre::Viewport for this world.
	Ogre::Viewport* getViewport(void) const;

private:
	// Ogre3D.
	Ogre::SceneManager* m_scene;
	Ogre::Viewport*	m_viewport;

	// Component pools.
	std::shared_ptr<ComponentPool<CameraComponent>> m_cameraComponentPool;
	std::shared_ptr<ComponentPool<FirstPersonComponent>> m_firstPersonComponentPool;
	std::shared_ptr<ComponentPool<InputComponent>> m_inputComponentPool;
	std::shared_ptr<ComponentPool<ModelComponent>> m_modelComponentPool;
	std::shared_ptr<ComponentPool<SceneComponent>> m_sceneComponentPool;

	// Entity pool.
	std::shared_ptr<EntityPool> m_entityPool;
};

// ========================================================================= //

// Component factory functions:

inline CameraComponentPtr World::createCameraComponent(void){
	return m_cameraComponentPool->create();
}

inline FirstPersonComponentPtr World::createFirstPersonComponent(void){
	return m_firstPersonComponentPool->create();
}

inline InputComponentPtr World::createInputComponent(void){
	return m_inputComponentPool->create();
}

inline ModelComponentPtr World::createModelComponent(void){
	return m_modelComponentPool->create();
}

inline SceneComponentPtr World::createSceneComponent(void){
	return m_sceneComponentPool->create();
}

// Getters:

inline Ogre::SceneManager* World::getSceneManager(void) const{
	return m_scene;
}

inline Ogre::Viewport* World::getViewport(void) const{
	return m_viewport;
}

// ========================================================================= //

#endif

// ========================================================================= //