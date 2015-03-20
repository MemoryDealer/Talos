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
// File: Pool.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Pool class.
// ========================================================================= //

#ifndef __POOL_HPP__
#define __POOL_HPP__

// ========================================================================= //

class AbstractPool
{
public:
    virtual ~AbstractPool(void) = 0 { }    
};

// ========================================================================= //
// Generic class for pooling objects in contiguous memory.
template<typename T>
class Pool : public AbstractPool
{
public:
    // Allocates pool and in use bits.
    explicit Pool(const uint32_t size);

    // Frees pool and in use bits.
    virtual ~Pool(void) override;

    // Returns next available object from pool.
    T* create(void);

    // Getters:

    // Returns size of pool.
    const uint32_t getSize(void) const;

    // Returns internal pool.
    T* getPool(void) const;

private:
    T* m_pool;
    bool* m_inUse;
    int m_numActive;
    uint32_t m_size;
};

// ========================================================================= //

#endif

// ========================================================================= //