// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
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