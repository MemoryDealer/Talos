// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: InputComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines InputComponent class.
// ========================================================================= //

#ifndef __INPUTCOMPONENT_HPP__
#define __INPUTCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Processes input using SDL library.
class InputComponent : public Component
{
public:
	// Sets the Component name.
	explicit InputComponent(void);

	// Empty destructor.
	virtual ~InputComponent(void) override;

	// Empty.
	virtual void init(EntityPtr, World&) override;

	// Empty.
	virtual void destroy(EntityPtr, World&) override;

	// Detects input and messages the Entity.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

private:

};

// ========================================================================= //

#endif

// ========================================================================= //