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
// File: CommandRepository.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines CommandRepository struct.
// ========================================================================= //

#ifndef __COMMANDREPOSITORY_HPP__
#define __COMMANDREPOSITORY_HPP__

// ========================================================================= //

#include "CommandTypes.hpp"
#include "stdafx.hpp"

// ========================================================================= //

typedef std::unordered_map<CommandType, CommandPtr> CommandTable;

// ========================================================================= //
// A single location to statically store all Command instances needed by an 
// actor. Each CommandPtr is stored in a hash table.
// (note: this uses the Flyweight pattern)
class CommandRepository
{
public:
    // Allocates all Command pointers, inserts them into table.
    explicit CommandRepository(void);

    // Deletes all Command pointers in table.
    ~CommandRepository(void);

    // Adds command into internal hash table.
    void addCommand(CommandPtr command);

    // Returns pointer to Command of specified type, if in table.
    CommandPtr getCommand(const CommandType type);

private:
    CommandTable m_commands;
};

// ========================================================================= //

#endif

// ========================================================================= //