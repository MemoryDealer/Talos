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

class PhysicsComponent : public Component
{
public:
    // Default initializes member data, stores mesh filename.
    explicit PhysicsComponent(void);

    // Empty destructor.
    virtual ~PhysicsComponent(void) override;

    enum class Type{
        Static,
        Dynamic
    };

    // Empty.
    virtual void init(World&) override;

    // Initializes PhysX actor, adds to World's PxScene.
    virtual void init(World&, 
                      EntityPtr,
                      const Type,
                      PxGeometry&,
                      const PxReal staticFriction = 0.5f, 
                      const PxReal dynamicFriction = 0.5f, 
                      const PxReal restitution = 0.3f,
                      const PxReal density = 10.f);

    // Removes PhysX actor from World's PxScene.
    virtual void destroy(World&) override;

    // Retrieves the actor's position and orientation, applies them to the
    // attached SceneComponent for rendering.
    virtual void update(World&) override;

    // Empty.
    virtual void message(const ComponentMessage&) override;

    // Component functions:

    // Sets PhysX pose position to these coordinates.
    void setPosition(const PxReal, const PxReal, const PxReal);

    // Sets PhysX pose orientation to this quaternion.
    void setOrientation(const PxReal, const PxReal, const PxReal, 
                        const PxReal);

    // Translates the PhysX pose directly.
    void translate(const PxReal, const PxReal, const PxReal);

    // Rotates the PhysX pose by this vector.
    void rotate(const PxReal, const PxReal, const PxReal);

    // Getters:

    // Returns position in form of Ogre::Vector3.
    const Ogre::Vector3 getPosition(void) const;

    // Returns position in form of Ogre::Quaternion.
    const Ogre::Quaternion getOrientation(void) const;

    // Returns pointer to PxRigidActor.
    PxRigidActor* getRigidActor(void) const;

    // Returns pointer to PxRigidStatic.
    PxRigidStatic* getStaticActor(void) const;

    // Returns pointer to PxRigidDynamic.
    PxRigidDynamic* getDynamicActor(void) const;

private:
    union{
        PxRigidStatic* m_sActor;
        PxRigidDynamic* m_dActor;
    };

    PxRigidActor* m_actor;
    PxMaterial* m_mat;
};

// ========================================================================= //

// Getters:

inline PxRigidActor* PhysicsComponent::getRigidActor(void) const{
    return m_actor;
}

inline PxRigidStatic* PhysicsComponent::getStaticActor(void) const{
    return m_sActor;
}

inline PxRigidDynamic* PhysicsComponent::getDynamicActor(void) const{
    return m_dActor;
}

// ========================================================================= //

#endif

// ========================================================================= //