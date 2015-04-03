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
// File: AttackFlare.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines AttackFlare class.
// ========================================================================= //

#ifndef __ATTACKFLARE_HPP__
#define __ATTACKFLARE_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //
// Displays a flare from a weapon attack (e.g., muzzle flare for gun, spark 
// from sword hitting surface).
class AttackFlare final
{
public:
    // Default initializes member data.
    explicit AttackFlare(void);

    // Empty destructor.
    ~AttackFlare(void);

    // Creates data to animate attack flare from config file.
    void init(std::shared_ptr<World> world, 
              Ogre::SceneNode* parent, 
              const std::string& cfg);

    // Shows attack flare if not already active.
    void activate(void);

    // Processes flare time.
    void update(void);

private:
    std::shared_ptr<World> m_world;
    Ogre::SceneNode* m_node;
    Ogre::BillboardSet* m_bbSet;
    Ogre::Billboard* m_bb;
    Ogre::Light* m_light;
    Ogre::Timer m_timer;
    Ogre::Real m_scale;
    Ogre::Real m_life, m_rate, m_max;
    bool m_active;
};

// ========================================================================= //

#endif

// ========================================================================= //