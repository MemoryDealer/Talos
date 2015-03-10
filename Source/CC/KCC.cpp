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
#include "Core/Talos.hpp"
#include "World/World.hpp"

// ========================================================================= //

KCC::KCC(void) :
m_controller(nullptr),
m_yVel(0.f),
m_onSurface(false),
m_jumping(false)
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

    if (!m_controller){
        throw std::exception("PhysX character controller failed!");
    }

    return true;
}

// ========================================================================= //

void KCC::destroy(World& world)
{
    // ?
}

// ========================================================================= //

PxExtendedVec3 KCC::update(World& world, 
                           const PxReal dx, 
                           const PxReal dy, 
                           const PxReal dz)
{
    const PxReal gravity = 0.0281f;
    const PxReal maxY = -7.81f;
   
    

    m_yVel -= gravity;
    if (m_yVel < maxY){
        m_yVel = maxY;
    }



    PxControllerState state;
    m_controller->getState(state);
    if (state.touchedActor){
       
        
        
    }


    PxVec3 disp(dx, m_yVel, dz);
    
    m_controller->move(disp, 0.001f, 1.f / Talos::MS_PER_UPDATE, 0);

    const PxReal surfaceMax = 1.32f;
    PxExtendedVec3 pos = m_controller->getPosition();
    PxVec3 origin(pos.x, pos.y, pos.z);
    PxVec3 uDir = PxVec3(0.f, -1.f, 0.f);
    PxReal maxDist = 99999.f;
    PxRaycastBuffer hit;
    PxQueryFilterData fd;
    fd.flags |= PxQueryFlag::eANY_HIT;
    bool status = world.getPScene()->getScene()->raycast(origin,
                                                         uDir,
                                                         maxDist,
                                                         hit,
                                                         PxHitFlags(PxHitFlag::eDEFAULT),
                                                         fd);
    if (status){
        if (hit.hasBlock){
            printf("Dist: %.2f", hit.block.distance);
            printf("\t%s\n", hit.block.actor->getConcreteTypeName());

            if (hit.block.distance <= surfaceMax){
                m_onSurface = true;
                m_yVel = 0.f;
                m_jumping = false;
            }
            else{
                m_onSurface = false;
            }
        }
    }
    
    // Get updated position.
    pos = m_controller->getPosition();
    // Adjust for head height.
    pos.y += 0.8644f;
    return pos;
}

// ========================================================================= //

void KCC::jump(void)
{
    // See if standing on a surface.
    /*PxVec3 origin(pos.x, pos.y, pos.z);
    PxVec3 uDir = PxVec3(0.f, -1.f, 0.f);
    PxReal maxDist = 10.f;
    PxRaycastBuffer hit;
    PxQueryFilterData fd;
    fd.flags |= PxQueryFlag::eANY_HIT;
    bool status = world.getPScene()->getScene()->raycast(origin,
                                                         uDir,
                                                         maxDist,
                                                         hit,
                                                         PxHitFlags(PxHitFlag::eDEFAULT),
                                                         fd);
    if (status){
        printf("Dist: %.2f\n", hit.block.distance);
        if (hit.block.distance < 0.1f){
            m_onSurface = true;
        }
        else{
            m_onSurface = false;
        }
    }*/
    //m_onSurface = true;
    if (!m_jumping){
        if (m_onSurface){
            m_jumping = true;
            m_yVel = 0.8f;
        }
    }
}

// ========================================================================= //