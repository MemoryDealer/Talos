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
// File: PhysicsComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines PhysicsComponent class.
// ========================================================================= //

#ifndef __PHYSICSCOMPONENT_HPP__
#define __PHYSICSCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //
// A dynamic actor in the physics simulation.
class PhysicsComponent : public Component
{
public:
    // Default initializes member data.
    explicit PhysicsComponent(void);

    // Empty destructor.
    virtual ~PhysicsComponent(void) override;

    enum class Type{
        Box,
        Sphere,
        Mesh
    };

    // Empty destructor.
    virtual void init(void) override;

    // Initializes PhysX actor, adds to World's PxScene.
    virtual void init(EntityPtr entity);

    // Removes PhysX actor from World's PxScene.
    virtual void destroy(void) override;

    // Retrieves the actor's position and orientation, applies them to the
    // attached SceneComponent for rendering.
    virtual void update(void) override;

    // Empty.
    virtual void message(ComponentMessage& msg) override;

    // Component functions:

    // Translates the PhysX pose directly.
    void translate(const PxReal dx, 
                   const PxReal dy, 
                   const PxReal dz);

    // Rotates the PhysX pose by this vector.
    void rotate(const PxReal rx, 
                const PxReal ry, 
                const PxReal rz);

    // Getters:

    // Returns position in form of Ogre::Vector3.
    const Ogre::Vector3 getPosition(void) const;

    // Returns position in form of Ogre::Quaternion.
    const Ogre::Quaternion getOrientation(void) const;

    // Returns pointer to PxRigidActor.
    PxRigidDynamic* getRigidActor(void) const;

    // Setters:

    // Sets PhysX pose position to these coordinates.
    void setPosition(const PxReal x,
                     const PxReal y,
                     const PxReal z);

    // Sets PhysX pose orientation to this quaternion.
    void setOrientation(const PxReal w,
                        const PxReal x,
                        const PxReal y,
                        const PxReal z);

    // Sets collision volume type.
    void setType(const Type& type);

    // Sets material value for dynamic actor.
    void setMaterial(World& world,
                     const PxReal staticFriction,
                     const PxReal dynamicFriction,
                     const PxReal restitution);

    // Sets density value for dynamic actor.
    void setDensity(const PxReal density);

private:
    PxRigidDynamic* m_rigidActor;
    Type m_type;
    PxMaterial* m_mat;
    PxReal m_density;
};

// ========================================================================= //

#endif

// ========================================================================= //