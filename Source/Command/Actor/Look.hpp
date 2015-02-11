// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Look.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Look class.
// ========================================================================= //

#ifndef __LOOK_HPP__
#define __LOOK_HPP__

// ========================================================================= //

#include "Command/Command.hpp"
#include "Component/ActorComponent.hpp"

// ========================================================================= //

class LookCommand;

typedef std::shared_ptr<LookCommand> LookCommandPtr;

// ========================================================================= //

class LookCommand : public Command
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

inline void LookCommand::setXY(const int relx, const int rely){
	m_relx = relx;
	m_rely = rely;
}

// ========================================================================= //

#endif

// ========================================================================= //