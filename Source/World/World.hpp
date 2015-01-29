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

class System;

typedef std::vector<std::shared_ptr<System>> SystemList;

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

	// Getters:

	// Returns pointer to Ogre::SceneManager for this world.
	Ogre::SceneManager* getSceneManager(void) const;

	// Returns pointer to the Ogre::Viewport for this world.
	Ogre::Viewport* getViewport(void) const;

private:
	Ogre::SceneManager* m_scene;
	Ogre::Viewport*	m_viewport;
	SystemList m_systems;
};

// ========================================================================= //

inline Ogre::SceneManager* World::getSceneManager(void) const{
	return m_scene;
}

inline Ogre::Viewport* World::getViewport(void) const{
	return m_viewport;
}

// ========================================================================= //

#endif

// ========================================================================= //