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
// File: Physics.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Physics class.
// ========================================================================= //

#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //
// Holds top-level PhysX objects for initialization of the physics engine.
class Physics final
{
    friend class PScene;

public:
    explicit Physics(void);

    ~Physics(void);

    const bool init(void);

    void destroy(void);

    // Talos conversion functions:

    static const EntityID toEntityID(const PxRigidActor* actor){
        return reinterpret_cast<const EntityID>(
            static_cast<void*>(actor->userData));
    }

    // Ogre-PhysX Conversion functions:

    static Ogre::Vector3 toOgre(const PxVec3 &vec3)
    {
        return Ogre::Vector3(vec3.x, vec3.y, vec3.z);
    }

    static Ogre::Quaternion toOgre(const PxQuat &q)
    {
        return Ogre::Quaternion(q.w, q.x, q.y, q.z);
    }

    static Ogre::AxisAlignedBox toOgre(const PxBounds3 &b)
    {
        return Ogre::AxisAlignedBox(toOgre(b.minimum), toOgre(b.maximum));
    }

    static PxVec3 toPx(const Ogre::Vector3 &vec3)
    {
        return PxVec3(vec3.x, vec3.y, vec3.z);
    }

    static PxQuat toPx(const Ogre::Quaternion &q)
    {
        PxQuat pxq; pxq.w = q.w; pxq.x = q.x; pxq.y = q.y; pxq.z = q.z;
        return pxq;
    }

    static PxBounds3 toPx(const Ogre::AxisAlignedBox &b)
    {
        PxBounds3 Pxb;
        Pxb.minimum = toPx(b.getMinimum());
        Pxb.maximum = toPx(b.getMaximum());
        return Pxb;
    }

    static PxMat44 toPx(const Ogre::Vector3 &pos, const Ogre::Quaternion &rot)
    {
        return PxMat44(PxMat33(toPx(rot)), toPx(pos));
    }

private:
    PxFoundation* m_foundation;
    PxPhysics* m_physx;
    PxDefaultAllocator m_defaultAllocator;
    PxDefaultErrorCallback m_defaultErrorCallback;
    PxVisualDebuggerConnection* m_debuggerConnection;
    PxCooking* m_cookingInterface;
};

// ========================================================================= //

#endif

// ========================================================================= //