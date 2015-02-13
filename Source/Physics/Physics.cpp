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
// File: Physics.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Physics class.
// ========================================================================= //

#include "Physics.hpp"

// ========================================================================= //

Physics::Physics(void) :
m_foundation(nullptr),
m_physx(nullptr),
m_defaultAllocator(),
m_defaultErrorCallback(),
m_debuggerConnection(nullptr)
{
	
}

// ========================================================================= //

Physics::~Physics(void)
{

}

// ========================================================================= //

const bool Physics::init(void)
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION,
									  m_defaultAllocator,
									  m_defaultErrorCallback);

	PxTolerancesScale scale;
	scale.length = 100;
	scale.mass = 1000;
	scale.speed = 10;

	bool recordMemoryAllocations = true;
	m_physx = PxCreatePhysics(PX_PHYSICS_VERSION,
								*m_foundation,
								PxTolerancesScale(),
								recordMemoryAllocations,
								nullptr);
	if (m_physx == nullptr){
		return false;
	}

	/*const char* host = "127.0.0.1";
	const int port = 5425;
	const unsigned int timeout = 100;

	PxVisualDebuggerConnectionFlags connectionFlags = 
		PxVisualDebuggerExt::getAllConnectionFlags();

	m_debuggerConnection = PxVisualDebuggerExt::createConnection(
		m_physx->getPvdConnectionManager(),
		host,
		port,
		timeout,
		connectionFlags);*/

	return true;
}

// ========================================================================= //

void Physics::destroy(void)
{
	/*m_debuggerConnection->release();*/

	m_physx->release();
	m_foundation->release();
}

// ========================================================================= //