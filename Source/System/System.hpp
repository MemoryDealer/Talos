// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: System.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines System class.
// ========================================================================= //

#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //
// Interface class for all Systems.
class System
{
public:
	// Empty constructor.
	explicit System(void) { }

	// Empty destructor.
	virtual ~System(void) = 0;

	// Updates each active component.
	virtual void update(void) = 0;
};

// ========================================================================= //

#endif

// ========================================================================= //