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
// File: System.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements System class.
// ========================================================================= //

#include "Entity/Entity.hpp"
#include "System.hpp"

// ========================================================================= //

System::System(void) :
m_entities(),
m_world(nullptr)
{

}

// ========================================================================= //

System::~System(void)
{

}

// ========================================================================= //

void System::attachEntity(EntityPtr entity)
{
    m_entities[entity->getID()] = entity;
    this->onEntityAttached(entity);
}

// ========================================================================= //

void System::detachEntity(EntityPtr entity)
{
    m_entities.erase(entity->getID());
}

// ========================================================================= //