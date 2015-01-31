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
#include "Component/InputComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Entity/EntityPool.hpp"
#include "World.hpp"

// ========================================================================= //

World::World(void) :
m_scene(nullptr),
m_viewport(nullptr),
m_systems(),
m_entityPool(nullptr)
{

}

// ========================================================================= //

World::~World(void)
{

}

// ========================================================================= //

void World::init(Ogre::Root* root, Ogre::Viewport* viewport)
{
	m_scene = root->createSceneManager(Ogre::ST_GENERIC);
	m_viewport = viewport;

	// Allocate Entity pool.
	// @TODO: Read pool size from config file.
	m_entityPool.reset(new EntityPool(256));
}

// ========================================================================= //

void World::destroy(Ogre::Root* root)
{
	root->destroySceneManager(m_scene);
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

//template<typename T>
//T* World::createComponent(void)
//{
//
//}
//
//// Explicitly instantiate template functions for each Component type.
//template CameraComponentPtr World::createComponent<CameraComponent>(void);

// ========================================================================= //

void World::update(void)
{
	for (int i = 0; i < m_entityPool->m_poolSize; ++i){
		m_entityPool->m_pool[i].update(*this);
	}
}

// ========================================================================= //