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