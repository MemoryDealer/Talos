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

#include "Command/CommandTypes.hpp"
#include "KCC.hpp"
#include "Physics/PScene.hpp"
#include "Core/Talos.hpp"
#include "World/World.hpp"

// ========================================================================= //

KCC::KCC(void) :
m_controller(nullptr),
m_lastMove(0.f, 0.f, 0.f),
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

const PxExtendedVec3 KCC::move(const Ogre::Vector3& translate)
{
    PxVec3 disp(translate.x, 0.f, translate.z);
    m_lastMove += disp;
    
    m_controller->move(disp, 0.1f, 1.f / Talos::MS_PER_UPDATE, 0);

    return m_controller->getPosition();
}

// ========================================================================= //

const PxExtendedVec3 KCC::update(World& world)
{
    // Free-fall/gravity values.
    const PxReal gravity = 0.0281f; // Amount to advance y-velocity down.
    const PxReal maxY = -7.81f; // Maximum negative y-velocity.

    // Apply gravity.
    m_yVel -= gravity;

    // Lock velocity at max.
    if (m_yVel < maxY){
        m_yVel = maxY;
    }

    PxVec3 disp(0.f, m_yVel, 0.f);
    /*disp.normalize();
    disp *= 0.25f;*/
    const PxReal mag = m_lastMove.magnitudeSquared();
    //printf("mag: %.2f\n", mag);
    if (mag > 0.04f){
        const PxReal f = 5.f;
        disp.x = -m_lastMove.x * mag * f;
        disp.z = -m_lastMove.z * mag * f;
        printf("Adjusting: %.2f, %.2f\n", disp.x, disp.z);
    }
    

    m_lastMove.x = m_lastMove.y = m_lastMove.z = 0.f;

    
    // Move the kinematic actor.
    m_controller->move(disp, 0.1f, 1.f / Talos::MS_PER_UPDATE, 0);
 
    // Get position for raycast origin.
    PxExtendedVec3 pos = m_controller->getPosition();

    // Perform a raycast straight down.
    PScene::Ray ray;
    ray.origin = PxVec3(pos.x, pos.y, pos.z);
    bool status = world.getPScene()->raycast(ray);

    // If there was a hit, process it.
    if (status){
        if (ray.hit.hasBlock){
            // Determine if player is standing on a surface.
            const PxReal surfaceMax = 1.32f; 
            if (ray.hit.block.distance <= surfaceMax){
                m_onSurface = true;
                m_yVel = 0.f;
                m_jumping = false;
            }
            else{
                m_onSurface = false;
            }
        }
    }

    // If jumping and the player hits a ceiling, stop the jump.
    // @TODO: raycast still returns result if no hit (distance = 0.f)
    //if (m_jumping){
    //    // Cast ray upwards.
    //    PScene::Ray ray;
    //    ray.origin = PxVec3(pos.x, pos.y, pos.z);
    //    ray.dir = PxVec3(0.f, 1.f, 0.f);
    //    ray.flags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC;
    //    bool status = world.getPScene()->raycast(ray);

    //    if (status){
    //        if (ray.hit.hasAnyHits()){
    //            
    //            printf("Ray dist: %.2f\n", ray.hit.block.distance);
    //            // If a ceiling has been hit.
    //            if (ray.hit.block.distance != 0.f && 
    //                ray.hit.block.distance < 2.f){
    //                m_yVel = -gravity;
    //                m_jumping = false;
    //            }
    //        }
    //    }
    //}
    
    // Adjust y-value for head height.
    //pos.y += 0.8644f;
    return pos;
}

// ========================================================================= //

void KCC::jump(void)
{
    if (!m_jumping){
        if (m_onSurface){
            m_jumping = true;
            m_yVel = 0.8f;
        }
    }
}

// ========================================================================= //