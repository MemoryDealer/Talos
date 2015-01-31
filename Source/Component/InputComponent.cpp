// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: InputComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements InputComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "Entity/Entity.hpp"
#include "InputComponent.hpp"

// ========================================================================= //

InputComponent::InputComponent(void) :
Component()
{
	this->setName("InputComponent");
}

// ========================================================================= //

InputComponent::~InputComponent(void)
{

}

// ========================================================================= //

void InputComponent::init(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void InputComponent::destroy(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void InputComponent::update(EntityPtr entity, World& world)
{
	// Poll SDL for input events.
	SDL_Event e;
	ComponentMessage msg;
	while (SDL_PollEvent(&e)){
		switch (e.type){
		default:
			break;

		case SDL_MOUSEMOTION:
			msg.type = ComponentMessageType::INPUT_MOUSE_MOTION;
			// Store mouse's relative movement.
			msg.mouse.x = e.motion.xrel;
			msg.mouse.y = e.motion.yrel;
			break;

		case SDL_KEYDOWN:

			break;
		}
	}

	entity->message(msg);
}

// ========================================================================= //

void InputComponent::message(const ComponentMessage& msg)
{

}

// ========================================================================= //