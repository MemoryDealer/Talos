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
// File: Ocean.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Ocean interface.
// ========================================================================= //

#ifndef __OCEAN_HPP__
#define __OCEAN_HPP__

// ========================================================================= //
// Interface for rendering a large (visually infinite) body of water.
class Ocean
{
public:
    explicit Ocean(void) { }

    virtual ~Ocean(void) = 0 { }

    virtual void destroy(void) = 0;

    virtual void update(void) = 0;

    // Setters:

    virtual void setPosition(const Ogre::Real, 
                             const Ogre::Real, 
                             const Ogre::Real) { }

    virtual void setSunPosition(const Ogre::Vector3&) { }

    virtual void setSunColour(const Ogre::Vector3&) { }

    virtual void setSunColour(const Ogre::ColourValue&) { }

    virtual void setSunEnabled(const bool) { }
};

// ========================================================================= //

#endif

// ========================================================================= //