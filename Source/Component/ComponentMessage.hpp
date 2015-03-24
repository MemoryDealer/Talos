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
// File: ComponentMessage.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ComponentMessage struct for Components.
// ========================================================================= //

#ifndef __COMPONENTMESSAGE_HPP__
#define __COMPONENTMESSAGE_HPP__

// ========================================================================= //

#include "Command/CommandTypes.hpp"
#include "Network/Update.hpp"
#include "stdafx.hpp"

// ========================================================================= //

// @TODO: Find a better place to put these structs.

struct ControllerAxisMotion{
    Sint16 x1; // x-position of left joystick.
    Sint16 y1; // y-position of left joystick.
    Sint16 x2; // x-position of right joystick.
    Sint16 y2; // y-position of right joystick.
};


struct MouseMove{
    int32_t relx;
    int32_t rely;
};
// ========================================================================= //

struct ComponentMessage{

    enum class Type{
        Null = 0,

        // Direct positional functions.
        GetPosition,
        SetPosition,
        GetOrientation,
        SetOrientation,
        Translate,
        TransformUpdate,

        // Player-world interaction functions (input driven).
        Move,
        Look,
        Action,
        Command,

        // Entity-world interaction functions.
        LinkActivate,                
        
        End
    };

    explicit ComponentMessage(const Type type = Type::Null) : type(type) { }

    Type type;

    boost::variant<
        std::string, 
        Ogre::Vector3,
        Ogre::Quaternion,
        ControllerAxisMotion,
        MouseMove,
        CommandType,
        TransformUpdate
        > data;
};

// ========================================================================= //

#endif

// ========================================================================= //