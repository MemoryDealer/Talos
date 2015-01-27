// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: HelperFunctions.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines some useful helper functions.
// ========================================================================= //

#ifndef __HELPERFUNCTIONS_HPP__
#define __HELPERFUNCTIONS_HPP__

// ========================================================================= //

#include <string>

// ========================================================================= //

template<typename T>
static std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

// ========================================================================= //

#endif

// ========================================================================= //