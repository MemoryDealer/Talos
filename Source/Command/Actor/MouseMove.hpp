// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: MouseMove.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines MouseMoveCommand class.
// ========================================================================= //

#ifndef __MOUSEMOVE_HPP__
#define __MOUSEMOVE_HPP__

// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/ActorComponent.hpp"

// ========================================================================= //

class MouseMoveCommand;

typedef std::shared_ptr<MouseMoveCommand> MouseMoveCommandPtr;

// ========================================================================= //

class MouseMoveCommand : public Command
{
public:
	void setXY(const int relx, const int rely);

	virtual void execute(EntityPtr entity) override{
		ActorComponentPtr actor = entity->getActorComponent();

		actor->look(m_relx, m_rely);
	}

private:
	int m_relx, m_rely;
};

// ========================================================================= //

inline void MouseMoveCommand::setXY(const int relx, const int rely){
	m_relx = relx;
	m_rely = rely;
}

// ========================================================================= //

#endif

// ========================================================================= //