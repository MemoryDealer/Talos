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

#include "Command/CommandRepository.hpp"
#include "Component/ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "Input.hpp"

// ========================================================================= //

Input::Input(void) :
m_player(nullptr),
m_gui(nullptr),
m_commandRepo(new CommandRepository()),
m_keymap()
{
	// @TODO: For every key on keyboard read from keymap file
	// (add mapKey() function which is a switch-case on the command).
	for (unsigned int i = 0; i < SDLK_z; ++i){
		m_keymap.insert(KM_VT(i, m_commandRepo->NullCommand));
	}
	m_keymap.insert(KM_VT(SDLK_LALT, m_commandRepo->NullCommand));

	m_keymap.find(SDLK_w)->second = m_commandRepo->MoveForwardCommand;
	m_keymap.find(SDLK_s)->second = m_commandRepo->MoveBackwardCommand;
	m_keymap.find(SDLK_a)->second = m_commandRepo->MoveLeftCommand;
	m_keymap.find(SDLK_d)->second = m_commandRepo->MoveRightCommand;
}

// ========================================================================= //

Input::~Input(void)
{

}

// ========================================================================= //

const CommandPtr Input::handle(const SDL_Event& e)
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

	case SDL_KEYDOWN:
		switch (e.key.keysym.sym){
		default:
			return m_keymap.find(e.key.keysym.sym)->second;

		case SDLK_ESCAPE:
			return nullptr;
		}
		break;

	case SDL_KEYUP:
		switch (e.key.keysym.sym){
		default:
			return m_keymap.find(e.key.keysym.sym)->second;

		case SDLK_ESCAPE:
			return nullptr;
		}
		break;
	}

	return 0;
}

// ========================================================================= //