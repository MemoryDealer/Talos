// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
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
#include "Command/Actor/MouseMove.hpp"
#include "Command/Actor/MoveBackward.hpp"
#include "Command/Actor/MoveForward.hpp"
#include "Command/Actor/MoveLeft.hpp"
#include "Command/Actor/MoveRight.hpp"
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
	MouseMoveCommandPtr MouseMoveCommand;
	CommandPtr MoveBackwardCommand;
	CommandPtr MoveForwardCommand;
	CommandPtr MoveLeftCommand;
	CommandPtr MoveRightCommand;
};

// ========================================================================= //

#endif

// ========================================================================= //