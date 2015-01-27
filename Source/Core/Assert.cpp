// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Assert.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Assert function for macro.
// ========================================================================= //

#include "stdafx.hpp"
#include "Assert.hpp"

// ========================================================================= //

const bool CustomAssert(const bool exp,
						const char* desc,
						const int line,
						const char* file)
{
	if (!exp){
		bool ret = false;
#ifdef WIN32
		std::string msg = std::string(desc) + "\r\nFILE: " + std::string(file)
			+ "\r\nLINE: " + toString(line) + "\r\nDo you wish to break?";
		if (static_cast<int>(MessageBox(GetForegroundWindow(), 
			msg.c_str(),
			"Assertion Triggered!", 
			MB_YESNO | MB_ICONEXCLAMATION)) == IDYES){
			ret = true;
		}

		return ret;
#endif
	}

	return false;
}

// ========================================================================= //