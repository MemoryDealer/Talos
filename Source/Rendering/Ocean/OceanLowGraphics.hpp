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
// File: OceanLowGraphics.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines OceanLowGraphics class.
// ========================================================================= //

#ifndef __OCEANLOWGRAPHICS_HPP__
#define __OCEANLOWGRAPHICS_HPP__

// ========================================================================= //

#include "Ocean.hpp"
#include "stdafx.hpp"

// ========================================================================= //
// Renders Ocean texture to a plane with some shading.
class OceanLowGraphics : public Ocean
{
public:
    explicit OceanLowGraphics(void);

    virtual ~OceanLowGraphics(void) override;

    void init(std::shared_ptr<World> world, 
              const std::string& material);

    virtual void destroy(void) override;

    virtual void update(void) override;

    // Setters:

    // Sets origin of plane.
    virtual void setPosition(const Ogre::Real,
                             const Ogre::Real,
                             const Ogre::Real) override;

private:
    Ogre::SceneNode* m_node;
    Ogre::Entity* m_entity;
    Ogre::Plane m_plane;

    std::shared_ptr<World> m_world;
};

// ========================================================================= //

#endif

// ========================================================================= //