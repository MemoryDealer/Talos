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
// File: StatComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines StatComponent class.
// ========================================================================= //

#ifndef __STATCOMPONENT_HPP__
#define __STATCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Holds stats for a player, monster, or for any need.
class StatComponent : public Component
{
public:
    // Default initializes member data.
    explicit StatComponent(void);

    virtual ~StatComponent(void) override;

    virtual void init(void) override;

    virtual void destroy(void) override;

    virtual void update(void) override;

    virtual void message(ComponentMessage& msg) override;

private:
    // @TODO: Use a polymorphic struct
    uint32_t m_hp;
};

// ========================================================================= //

#endif

// ========================================================================= //