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
// File: KCC.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements KCC class.
// ========================================================================= //

#include "KCC.hpp"
#include "Physics/PScene.hpp"
#include "World/World.hpp"

// ========================================================================= //

KCC::KCC(void) :
m_controller(nullptr)
{

}

// ========================================================================= //

KCC::~KCC(void)
{

}

// ========================================================================= //

bool KCC::init(World& world)
{
    PxCapsuleControllerDesc desc;
    desc.position = PxExtendedVec3(0.f, 0.f, 0.f);
    desc.height = 1.8288f;
    desc.radius = 0.3048f;

    PxMaterial* mat = world.getPScene()->getSDK()->createMaterial(0.5f,
                                                                  0.5f,
                                                                  0.1f);
    desc.material = mat;

    m_controller = world.getPScene()->getControllerManager()->
        createController(desc);

    if (m_controller == nullptr){
        return false;
    }

    return true;
}

// ========================================================================= //

void KCC::destroy(World& world)
{
    // ?
}

// ========================================================================= //

PxExtendedVec3 KCC::update(const PxReal dx, const PxReal dy, const PxReal dz)
{
    const PxReal gravity = -9.81f / 16.f; // Magic random value.
    PxVec3 disp(dx, gravity, dz);

    m_controller->move(disp, 0.001f, 16.f, 0);
    
    // Get updated position.
    PxExtendedVec3 pos = m_controller->getPosition();
    // Adjust for head height.
    pos.y += 0.8644f;
    return pos;
}

// ========================================================================= //