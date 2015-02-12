// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Pool.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Pool class.
// ========================================================================= //

#ifndef __POOL_HPP__
#define __POOL_HPP__

// ========================================================================= //
// Generic class for pooling objects in contiguous memory.
template<typename T>
class Pool
{
public:
	explicit Pool(const int size);

	~Pool(void);

	T* create(void);

private:
	T* m_pool;
	int m_numActive;
	int m_size;
};

// ========================================================================= //

#endif

// ========================================================================= //