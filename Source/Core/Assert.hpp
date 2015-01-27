// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Assert.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Assert macro.
// ========================================================================= //

#ifndef __ASSERT_HPP__
#define __ASSERT_HPP__

// ========================================================================= //
// Offers user a chance to break into the debugger when an assertion fails.
extern const bool CustomAssert(const bool, 
							   const char*, 
							   const int, 
							   const char*);

// ========================================================================= //

#ifdef _DEBUG
#define Assert(exp, desc)\
if (CustomAssert(static_cast<const bool>(exp), desc, __LINE__, __FILE__))\
	{ _asm { int 3 } } // Triggers break in debugger.
#else
#define Assert(exp, desc) ;
#endif

// ========================================================================= //

#endif

// ========================================================================= //