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
// File: EntityPool.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements EntityPool class.
// ========================================================================= //

#include "EntityPool.hpp"

// ========================================================================= //

EntityPool::EntityPool(const int size) :
m_pool(new Entity[size]),
m_firstAvail(&m_pool[0]),
m_idCounter(0),
m_poolSize(size)
{
	// Point each Entity to the next.
	for (int i = 0; i < size - 1; ++i){
		m_pool[i].setNext(&m_pool[i + 1]);
	}

	// Terminate the last one.
	m_pool[size - 1].setNext(nullptr);
}

// ========================================================================= //

EntityPool::~EntityPool(void)
{
	delete[] m_pool;
}

// ========================================================================= //

EntityPtr EntityPool::create(void)
{
	Assert(m_firstAvail != nullptr, "EntityPool firstAvail = nullptr");

	// O(1) time!
	EntityPtr e = m_firstAvail;
	m_firstAvail = e->getNext();

	e->setID(m_idCounter++);
	return e;
}

// ========================================================================= //

void EntityPool::destroy(EntityPtr e)
{
	e->setNext(m_firstAvail);
	m_firstAvail = e;
}

// ========================================================================= //
