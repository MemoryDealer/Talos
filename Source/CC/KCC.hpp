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
// File: KCC.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines KCC class.
// ========================================================================= //

#ifndef __KCC_HPP__
#define __KCC_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //

class World;

// ========================================================================= //
// Kinematic character controller using PhysX.
class KCC final
{
public:
    explicit KCC(void);

    ~KCC(void);

    bool init(World&);

    void destroy(World&);

    PxExtendedVec3 update(const PxReal, const PxReal, const PxReal);

    // Setters:

    void setPosition(const PxReal, const PxReal, const PxReal);

    void setPosition(const Ogre::Vector3&);

private:
    PxController* m_controller;
};

// ========================================================================= //

// Setters:

inline void KCC::setPosition(const PxReal x, 
                             const PxReal y, 
                             const PxReal z){
    m_controller->setPosition(PxExtendedVec3(x, y, z));
}

inline void KCC::setPosition(const Ogre::Vector3& p){
    this->setPosition(p.x, p.y, p.z);
}

// ========================================================================= //

#endif

// ========================================================================= //