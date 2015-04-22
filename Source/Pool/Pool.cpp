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

#include "Component/AllComponents.hpp"
#include "Pool.hpp"

// ========================================================================= //

// Explicitly instantiate ComponentPools.
template class Pool<ActorComponent>;
template class Pool<CameraComponent>;
template class Pool<CollisionComponent>;
template class Pool<LightComponent>;
template class Pool<LinkComponent>;
template class Pool<ModelComponent>;
template class Pool<MultiModelComponent>;
template class Pool<NetworkComponent>;
template class Pool<ParticleComponent>;
template class Pool<PhysicsComponent>;
template class Pool<RotationComponent>;
template class Pool<SceneComponent>;
template class Pool<SoundComponent>;
template class Pool<StatComponent>;
template class Pool<TrackComponent>;
template class Pool<WeaponComponent>;

// ========================================================================= //

template<typename T>
Pool<T>::Pool(const uint32_t size) :
m_pool(new T[size]),
m_inUse(new bool[size]),
m_numActive(0),
m_size(size)
{
    
}

// ========================================================================= //

template<typename T>
Pool<T>::~Pool(void)
{
    delete[] m_pool;
    delete[] m_inUse;
}

// ========================================================================= //

template<typename T>
T* Pool<T>::create(void)
{
    Assert(m_numActive < m_size, "Pool overflow!");

    return &m_pool[m_numActive++];
}

// ========================================================================= //

// Getters:

// ========================================================================= //

template<typename T>
const uint32_t Pool<T>::getSize(void) const
{
    return m_size;
}

// ========================================================================= //

template<typename T>
T* Pool<T>::getPool(void) const
{
    return m_pool;
}

// ========================================================================= //