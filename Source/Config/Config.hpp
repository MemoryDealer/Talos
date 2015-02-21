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
// File: Config.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Config class.
// ========================================================================= //

#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

namespace Talos
{; // Semicolon here as quick fix for visual studio's annoying indentation.

// ========================================================================= //
// A crude INI type config file parser.
class Config
{
public:
    // Default initializes member data.
    explicit Config(void);

    // Calls load() with file as argument.
    explicit Config(const std::string& file);

    // Empty destructor.
    virtual ~Config(void);

    // Opens the file in a std::ifstream.
    virtual bool load(const std::string& file);

    // Parses value in section, returns string containing value.
    virtual std::string& parseValue(const std::string& section, 
                                    const std::string& key);

    // Parses value in section, returns Ogre::Real containing value.
    virtual const Ogre::Real parseReal(const std::string& section,
                                       const std::string& key);

    // Parses value in section, returns int containing value.
    virtual const int parseInt(const std::string& section,
                               const std::string& key);

    // Parses value in section, returns bool containing value.
    virtual const bool parseBool(const std::string& section,
                                 const std::string& key);

    // Getters:

    // Returns true if file is loaded.
    const bool isLoaded(void) const;

private:
    std::ifstream m_file;
    std::string m_buffer;
    bool m_loaded;
};

// ========================================================================= //

inline const bool Config::isLoaded(void) const{
    return m_loaded;
}

// ========================================================================= //

}

// ========================================================================= //

#endif

// ========================================================================= //