// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
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

	virtual void execute(EntityPtr entity){
		return;
	}
};

// ========================================================================= //

#endif

// ========================================================================= //