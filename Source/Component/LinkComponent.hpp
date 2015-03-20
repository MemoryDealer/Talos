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
// File: LinkComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines LinkComponent class.
// ========================================================================= //

#ifndef __LINKCOMPONENT_HPP__
#define __LINKCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Can be used to link an entity to another, for things such as buttons, 
// switches, levers, etc.
class LinkComponent : public Component
{
public:
    explicit LinkComponent(void);

    virtual ~LinkComponent(void) override;

    // Empty.
    virtual void init(void) override;

    // Empty.
    virtual void destroy(void) override;

    // Empty.
    virtual void update(void) override;

    // Handles action messages, routes them to linked entity.
    virtual void message(ComponentMessage& msg);

    // Component functions:

    // Adds an EntityID to link this entity to.
    void addLinkID(const EntityID id);

private:
    std::vector<EntityID> m_linkedIDs;
};

// ========================================================================= //

#endif

// ========================================================================= //