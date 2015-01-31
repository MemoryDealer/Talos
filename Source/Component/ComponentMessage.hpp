// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Message.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Message struct for Components.
// ========================================================================= //

#ifndef __COMPONENTMESSAGE_HPP__
#define __COMPONENTMESSAGE_HPP__

// ========================================================================= //

enum ComponentMessageType{
	NIL = 0,

	INPUT_MOUSE_MOTION,

	END
};

// ========================================================================= //

struct ComponentMessage{

	explicit ComponentMessage(void) : type(ComponentMessageType::NIL) { }

	int type;

	union{
		struct{
			int x;
			int y;
		} mouse;
	};
};

// ========================================================================= //

#endif

// ========================================================================= //