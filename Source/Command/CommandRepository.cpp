// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: CommandRepository.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements CommandRepository struct.
// ========================================================================= //

#include "CommandRepository.hpp"

// ========================================================================= //

CommandRepository::CommandRepository(void) :
NullCommand(new ::NullCommand()),
MoveBackwardCommand(new ::MoveBackwardCommand()),
MoveForwardCommand(new ::MoveForwardCommand()),
MoveLeftCommand(new ::MoveLeftCommand()),
MoveRightCommand(new ::MoveRightCommand())
{

}

// ========================================================================= //

CommandRepository::~CommandRepository(void)
{

}

// ========================================================================= //