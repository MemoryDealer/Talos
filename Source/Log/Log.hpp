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
// File: Log.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Log class.
// ========================================================================= //

#ifndef __LOG_HPP__
#define __LOG_HPP__

// ========================================================================= //

#include <Ogre.h>

// ========================================================================= //

namespace Talos
{;

// ========================================================================= //
// Currently just a wrapper for logging to the Ogre log file.
class Log : public Ogre::Singleton<Log>
{
public:
    // Empty constructor.
    explicit Log(void);

    // Empty destructor.
    virtual ~Log(void);

    // Logs to Ogre log file with "Talos" prefix.
    virtual void log(const std::string& str);

private:

};

// ========================================================================= //

}

// ========================================================================= //

#endif

// ========================================================================= //