// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Command.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Command class.
// ========================================================================= //

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

// ========================================================================= //

#include "Entity/Entity.hpp"

// ========================================================================= //

class Command;

typedef std::shared_ptr<Command> CommandPtr;

// ========================================================================= //

class Command
{
public:
	explicit Command(void) { }
	virtual ~Command(void) { }

	virtual void execute(EntityPtr) = 0;
	virtual void unexecute(EntityPtr) { }
};

// ========================================================================= //

#endif

// ========================================================================= //