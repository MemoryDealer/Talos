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
// File: Message.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Message struct for Components.
// ========================================================================= //

#ifndef __COMPONENTMESSAGE_HPP__
#define __COMPONENTMESSAGE_HPP__

// ========================================================================= //

enum ComponentMessageType{
    NIL = 0,

    INPUT_MOUSE_MOTION,

    END
};

// ========================================================================= //

struct ComponentMessage{

    explicit ComponentMessage(void) : type(ComponentMessageType::NIL) { }

    int type;

    union{
        struct{
            int x;
            int y;
            int absx;
            int absy;
        } mouse;
    };
};

// ========================================================================= //

#endif

// ========================================================================= //