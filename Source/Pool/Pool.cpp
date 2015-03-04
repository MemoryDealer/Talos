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
m_inUse(new bool[size]),
m_numActive(0),
m_size(size)
{
    
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