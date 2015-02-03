// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: World.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements World class.
// ========================================================================= //

#include "Component/CameraComponent.hpp"
#include "Component/FirstPersonComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/EntityPool.hpp"
#include "World.hpp"

// ========================================================================= //

World::World(void) :
m_root(nullptr),
m_scene(nullptr),
m_viewport(nullptr),
m_entityPool(nullptr),
m_cameraComponentPool(nullptr),
m_firstPersonComponentPool(nullptr),
m_modelComponentPool(nullptr),
m_sceneComponentPool(nullptr)
{
	
}

// ========================================================================= //

World::~World(void)
{

}

// ========================================================================= //

void World::init(void)
{
	m_scene = m_root->createSceneManager(Ogre::ST_GENERIC);

	// Allocate Entity pool.
	// @TODO: Read pool size from config file.
	m_entityPool.reset(new EntityPool(256));

	// Allocate Component pools.
	// @TODO: Read sizes from config file.
	m_cameraComponentPool.reset(new ComponentPool<CameraComponent>(5));
	m_firstPersonComponentPool.reset(new ComponentPool<FirstPersonComponent>
									 (5));
	m_modelComponentPool.reset(new ComponentPool<ModelComponent>(1024));
	m_sceneComponentPool.reset(new ComponentPool<SceneComponent>(1024));
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

void World::update(void)
{
	for (int i = 0; i < m_entityPool->m_poolSize; ++i){
		m_entityPool->m_pool[i].update(*this); // Dereference self.
	}
}

// ========================================================================= //