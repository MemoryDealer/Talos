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

#include "Command/NullCommand.hpp"
#include "Command/Actor/Look.hpp"
#include "Command/Actor/MoveBackward.hpp"
#include "Command/Actor/MoveForward.hpp"
#include "Command/Actor/MoveLeft.hpp"
#include "Command/Actor/MoveRight.hpp"
#include "Command/Actor/Debug/Spectator.hpp"
#include "stdafx.hpp"

// ========================================================================= //
// A single location to statically store all Command instances needed by an 
// actor. It's named a struct since all members are public.
// (note: this uses the Flyweight pattern)
struct CommandRepository
{
public:
	// Allocates all Command smart pointers.
	explicit CommandRepository(void);

	// Empty destructor (all smart pointers de-allocated).
	~CommandRepository(void);

	// Smart pointers to possible Commands.
	CommandPtr NullCommand;
	LookCommandPtr LookCommand;
	CommandPtr MoveBackwardCommand;
	CommandPtr MoveForwardCommand;
	CommandPtr MoveLeftCommand;
	CommandPtr MoveRightCommand;
	CommandPtr SpectatorCommand;
};

// ========================================================================= //

#endif

// ========================================================================= //