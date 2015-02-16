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
// File: Sky.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Sky class.
// ========================================================================= //

#ifndef __SKY_HPP__
#define __SKY_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

class Ocean;
class World;

// ========================================================================= //

class Sky final
{
public:
    explicit Sky(World& world,
                 const Graphics::Setting,
                 const std::string&);

    ~Sky(void);

    void update(void);

    // Getters:

    const Ogre::Real getTime(void) const;

    const Ogre::Vector3 getSunDirection(void) const;

    const Ogre::Vector3 getMoonDirection(void) const;

    const Ogre::Real calcSkydomeRadius(Ogre::Camera*) const;

private:
    union{
        // Low graphics.
        struct{

        };
        // High graphics.
        struct{
            SkyX::SkyX* m_skyX;
            SkyX::BasicController* m_basicController;            
        };
    };

    Graphics::Setting m_graphicsSetting;
};

// ========================================================================= //

inline const Ogre::Real Sky::getTime(void) const{
    return m_basicController->getTime().x;
}

inline const Ogre::Vector3 Sky::getSunDirection(void) const{
    return m_basicController->getSunDirection();
}

inline const Ogre::Vector3 Sky::getMoonDirection(void) const{
    return m_basicController->getMoonDirection();
}

// ========================================================================= //

#endif

// ========================================================================= //