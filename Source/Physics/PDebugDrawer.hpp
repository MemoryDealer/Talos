// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: PDebugDrawer.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines PDebugDrawer class.
// ========================================================================= //

#ifndef __PDEBUGDRAWER_HPP__
#define __PDEBUGDRAWER_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //

class DynamicLines;

// ========================================================================= //
// Draws registered PhysX actors using dynamic lines in Ogre3D.
class PDebugDrawer final
{
public:
	explicit PDebugDrawer(void);

	~PDebugDrawer(void);

	// Adds a PhysX actor to the debug drawing list.
	void add(PxRigidActor*, PxGeometry&);

	// Gets the pose of each PhysX actor and updates the lines.
	void update(void);

	struct Node{
		explicit Node(void) : lines(nullptr), actor(nullptr) { }

		std::shared_ptr<DynamicLines> lines;
		PxRigidActor* actor;
	};

private:
	std::vector<Node> m_nodes;
};

// ========================================================================= //

#endif

// ========================================================================= //