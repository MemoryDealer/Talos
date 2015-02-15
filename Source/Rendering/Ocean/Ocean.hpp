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
// Defines Ocean class.
// ========================================================================= //

#ifndef __OCEAN_HPP__
#define __OCEAN_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

class ActorComponent;
class World;

typedef ActorComponent* ActorComponentPtr;

// ========================================================================= //

class Ocean final
{
public:
    explicit Ocean(World&, 
                   const Graphics::Setting, 
                   const std::string&, 
                   ActorComponentPtr = nullptr);

    ~Ocean(void);

    void update(void);

    // Setters:

    void setPosition(const Ogre::Real, const Ogre::Real, const Ogre::Real);

private:
    union{
        // Low graphics.
        struct{

        };
        // High graphics.
        struct{
            Hydrax::Hydrax* m_hydrax;
            Ogre::Camera* m_hydraxCamera;
        };
    };

    // The actor which controls the camera.
    ActorComponentPtr m_actorC;
    Graphics::Setting m_graphicsSetting;
};

// ========================================================================= //

// Setters:

inline void Ocean::setPosition(const Ogre::Real x, 
                               const Ogre::Real y, 
                               const Ogre::Real z){
    switch (m_graphicsSetting){
    default:
        break;

    case Graphics::Setting::High:
        m_hydrax->setPosition(Ogre::Vector3(x, y, z));
        break;
    }
}

// ========================================================================= //

#endif

// ========================================================================= //