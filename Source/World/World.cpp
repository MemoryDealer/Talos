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
#include "Entity/Entity.hpp"

// ========================================================================= //

World::World(void) :
m_scene(nullptr),
m_viewport(nullptr),
m_systems(),
m_entityPool(),
m_numEntities(0)
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
	m_entityPool = new EntityPtr[256];
}

// ========================================================================= //

void World::destroy(Ogre::Root* root)
{
	root->destroySceneManager(m_scene);

	delete[] m_entityPool;
}

// ========================================================================= //

EntityPtr World::createEntity(void)
{
	return m_entityPool[m_numEntities++];
}

// ========================================================================= //