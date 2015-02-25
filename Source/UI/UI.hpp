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
// File: UI.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines UI interface.
// ========================================================================= //

#ifndef __UI_HPP__
#define __UI_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //
// Handles CEGUI creation and events.
class UI
{
public:
    // Empty constructor.
    explicit UI(void) { }

    virtual ~UI(void) = 0 { }

    virtual void init(void) = 0;

    virtual void destroy(void) = 0;

    virtual void update(void) = 0;
};

// ========================================================================= //

#endif

// ========================================================================= //