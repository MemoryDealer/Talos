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

#include "World.hpp"
#include "Entity/EntityPool.hpp"

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