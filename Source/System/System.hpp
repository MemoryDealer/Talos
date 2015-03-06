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
// File: System.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines System class.
// ========================================================================= //

#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

typedef std::unordered_map<EntityID, EntityPtr> EntityHashTable;

// ========================================================================= //
// Holds a collection of Entities which have related Components that require
// per-frame processing.
class System
{
public:
    // Default initializes member data.
    explicit System(void);

    // Empty destructor.
    virtual ~System(void) = 0;

    // Inserts EntityPtr into internal hash table.
    virtual void attachEntity(EntityPtr entity);

    // Removes EntityPtr from internal hash table.
    virtual void detachEntity(EntityPtr entity);

    // Processes related components.
    virtual void update(void) = 0;

protected:
    EntityHashTable m_entities;
};

// ========================================================================= //

#endif

// ========================================================================= //