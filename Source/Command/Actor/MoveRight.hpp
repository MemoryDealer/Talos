// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: MoveRight.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines MoveRightCommand class.
// ========================================================================= //

#ifndef __MOVERIGHT_HPP__
#define __MOVERIGHT_HPP__

// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/ActorComponent.hpp"

// ========================================================================= //

class MoveRightCommand : public Command
{
public:

	virtual void execute(EntityPtr entity){
		ActorComponentPtr actor = static_cast<ActorComponentPtr>(
			entity->getComponentPtr("ActorComponent"));

		actor->setMovingRight(true);
	}

	virtual void unexecute(EntityPtr entity){
		ActorComponentPtr actor = static_cast<ActorComponentPtr>(
			entity->getComponentPtr("ActorComponent"));

		actor->setMovingRight(false);
	}

};

// ========================================================================= //

#endif

// ========================================================================= //