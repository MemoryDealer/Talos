// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Input.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Input class.
// ========================================================================= //

#include "Component/ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "Input.hpp"

// ========================================================================= //

Input::Input(void) :
m_player(nullptr),
m_gui(nullptr)
{

}

// ========================================================================= //

Input::~Input(void)
{

}

// ========================================================================= //

void Input::handle(const SDL_Event& e)
{
	ComponentMessage msg;

	switch (e.type){
	default:
		break;

	case SDL_MOUSEBUTTONDOWN:

		break;

	case SDL_MOUSEBUTTONUP:

		break;

	case SDL_MOUSEMOTION:
		msg.type = ComponentMessageType::INPUT_MOUSE_MOTION;
		msg.mouse.x = e.motion.xrel;
		msg.mouse.y = e.motion.yrel;
		msg.mouse.absx = e.motion.x;
		msg.mouse.absy = e.motion.y;

		m_player->message(msg);
		break;
	}
}

// ========================================================================= //