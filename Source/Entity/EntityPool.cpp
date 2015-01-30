// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
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
m_idCounter(0)
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