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
// File: WeaponComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines WeaponComponent class.
// ========================================================================= //

#ifndef __WEAPONCOMPONENT_HPP__
#define __WEAPONCOMPONENT_HPP__

// ========================================================================= //

class AttackFlare;

#include "Component.hpp"

// ========================================================================= //
// A weapon an actor can wield.
class WeaponComponent : public Component
{
public:
    // Default initializes member data.
    explicit WeaponComponent(void);

    // Empty destructor.
    virtual ~WeaponComponent(void) override;

    // 
    virtual void init(void) override;

    virtual void destroy(void) override;

    virtual void update(void) override;

    virtual void message(ComponentMessage& msg) override;

    // Component functions:

    // Creates Ogre::Entity and SceneNode with weapon model under actor's
    // roll node.
    void setup(Ogre::SceneNode* actorRollNode);

    // Runs the weapon's attack procedure.
    void attack(void);

    // Performs ray test in direction weapon is facing.
    void hitscan(void);

    // Setters:

    // Sets the weapon to be rendered last if true. Needed for local 
    // player's weapon.
    void setClearDepth(const bool clear);

private:
    Ogre::SceneNode* m_node;
    Ogre::Entity* m_entity;
    bool m_clearDepth;

    std::shared_ptr<AttackFlare> m_attackFlare;

    // Weapon fire animation.
    Ogre::AnimationState* m_animationState;
};

// ========================================================================= //

#endif

// ========================================================================= //