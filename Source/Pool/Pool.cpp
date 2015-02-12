// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Pool.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Pool class.
// ========================================================================= //

#include "Component/ActorComponent.hpp"
#include "Component/CameraComponent.hpp"
#include "Component/LightComponent.hpp"
#include "Component/ModelComponent.hpp"
#include "Component/PhysicsComponent.hpp"
#include "Component/SceneComponent.hpp"
#include "Pool.hpp"

// ========================================================================= //

// Explicitly instantiate ComponentPools.
template class Pool<ActorComponent>;
template class Pool<CameraComponent>;
template class Pool<LightComponent>;
template class Pool<ModelComponent>;
template class Pool<PhysicsComponent>;
template class Pool<SceneComponent>;

// ========================================================================= //

template<typename T>
Pool<T>::Pool(const int size) :
m_pool(new T[size]),
m_numActive(0),
m_size(size){

}

template<typename T>
Pool<T>::~Pool(void)
{
	delete[] m_pool;
}

template<typename T>
T* Pool<T>::create(void)
{
	Assert(m_numActive < m_size, "Pool overflow!");

	return &m_pool[m_numActive++];
}

// ========================================================================= //