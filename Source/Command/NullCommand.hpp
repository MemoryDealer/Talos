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
// File: NullCommand.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines NullCommand class.
// ========================================================================= //

#ifndef __NULLCOMMAND_HPP__
#define __NULLCOMMAND_HPP__

// ========================================================================= //

#include "Command.hpp"

// ========================================================================= //
// Does nothing; the default CommandPtr assigned to all keys.
class NullCommand : public Command
{
public:
    explicit NullCommand(void){
        this->setType(CommandType::Null);
    }

    virtual void execute(EntityPtr entity) override{
        return;
    }
};

// ========================================================================= //

#endif

// ========================================================================= //