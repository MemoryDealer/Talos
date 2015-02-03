// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: ComponentPool.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ComponentPool class.
// ========================================================================= //

#ifndef __COMPONENTPOOL_HPP__
#define __COMPONENTPOOL_HPP__

// ========================================================================= //

#include "CameraComponent.hpp"
#include "FirstPersonComponent.hpp"
#include "ModelComponent.hpp"
#include "SceneComponent.hpp"

// ========================================================================= //

template<typename T>
class ComponentPool
{
public:
	explicit ComponentPool(const int size) :
		m_pool(new T[size]),
		m_numActive(0),
		m_size(size){ 
	
	}

	~ComponentPool(void){ 
		delete[] m_pool;
	}

	T* create(void){
		Assert(m_numActive < m_size, "ComponentPool overflow!");

		return &m_pool[m_numActive++];
	}

private:
	T* m_pool;
	int m_numActive;
	int m_size;
};

// ========================================================================= //

#endif

// ========================================================================= //