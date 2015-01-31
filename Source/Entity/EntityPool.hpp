// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: EntityPool.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines EntityPool class.
// ========================================================================= //

#ifndef __ENTITYPOOL_HPP__
#define __ENTITYPOOL_HPP__

// ========================================================================= //

#include "Entity.hpp"

// ========================================================================= //
// Stores Entity objects in contiguous memory and accesses them in constant
// time.
class EntityPool
{
	friend class World;

public:
	// Allocates pool array, sets the next pointer of each Entity.
	explicit EntityPool(const int size);

	// Deallocates pool array.
	~EntityPool(void);

	// Acquires the next available Entity, sets the ID and returns it.
	EntityPtr create(void);

	// Sets the next available Entity to the provided pointer.
	void destroy(EntityPtr);

private:
	EntityPtr m_pool;
	EntityPtr m_firstAvail;
	EntityID m_idCounter;
	int m_poolSize;
};

// ========================================================================= //

#endif

// ========================================================================= //