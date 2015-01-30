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

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

// ========================================================================= //

enum MessageType{
	NIL = 0,

	INPUT_MOUSE_MOTION,

	END
};

// ========================================================================= //

struct Message{
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