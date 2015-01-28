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

class Component;

// ========================================================================= //
// @TODO: redesign this class as an interface.
class System
{
public:
	// Default initializes component array.
	template<typename T>
	explicit System(const unsigned int size);

	// Empty destructor.
	~System(void);

	// Updates each active component.
	void update(void);

protected:
	//T m_components[N];
	std::vector<std::shared_ptr<Component>> m_components;
	unsigned int m_numActive;
};

// ========================================================================= //

#endif

// ========================================================================= //