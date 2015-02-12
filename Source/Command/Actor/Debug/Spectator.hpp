// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Spectator.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Spectator class.
// ========================================================================= //

#ifndef __SPECTATOR_HPP__
#define __SPECTATOR_HPP__

// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/ActorComponent.hpp"

// ========================================================================= //

class SpectatorCommand : public Command
{
public:
	
	virtual void execute(EntityPtr entity) override{
		ActorComponentPtr actor = entity->getActorComponent();

		actor->setMode(ActorComponent::Mode::SPECTATOR);
	}

	virtual void unexecute(EntityPtr entity) override{
		ActorComponentPtr actor = entity->getActorComponent();

		actor->setMode(ActorComponent::Mode::PLAYER);
	}
};

// ========================================================================= //

#endif

// ========================================================================= //