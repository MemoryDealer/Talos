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
// File: Physics.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Physics class.
// ========================================================================= //

#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //
// Holds top-level PhysX objects for initialization of the physics engine.
class Physics final
{
    friend class PScene;

public:
    explicit Physics(void);

    ~Physics(void);

    const bool init(void);

    void destroy(void);

private:
    PxFoundation* m_foundation;
    PxPhysics* m_physx;
    PxDefaultAllocator m_defaultAllocator;
    PxDefaultErrorCallback m_defaultErrorCallback;
    PxVisualDebuggerConnection* m_debuggerConnection;
};

// ========================================================================= //

#endif

// ========================================================================= //