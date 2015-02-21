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
// File: OceanHighGraphics.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements OceanHighGraphics class.
// ========================================================================= //

#include "OceanLowGraphics.hpp"
#include "World/World.hpp"

// ========================================================================= //

OceanLowGraphics::OceanLowGraphics(void)
{

}

// ========================================================================= //

OceanLowGraphics::~OceanLowGraphics(void)
{

}

// ========================================================================= //

void OceanLowGraphics::init(World* world, const std::string& material)
{
    m_world = world;

    const std::string meshName = "LowOcean";

    if (!Ogre::MeshManager::getSingleton().resourceExists(meshName)){
        m_plane.normal = Ogre::Vector3::UNIT_Y;
        m_plane.d = 0.f;
        Ogre::MeshManager::getSingleton().createPlane(
            meshName,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            m_plane,
            9999.f, 
            9999.f, 
            100, 
            100, 
            true, 
            1, 
            1.f, 
            1.f,
            Ogre::Vector3::UNIT_Z);
    }

    m_node = m_world->getSceneManager()->getRootSceneNode()->
        createChildSceneNode(meshName);

    m_entity = m_world->getSceneManager()->createEntity(meshName, 
                                                        meshName);
    m_node->attachObject(m_entity);

    m_entity->setMaterialName(material);
}

// ========================================================================= //

void OceanLowGraphics::destroy(void)
{
    m_world->getSceneManager()->destroySceneNode(m_node);
}

// ========================================================================= //

void OceanLowGraphics::update(void)
{

}

// ========================================================================= //

void OceanLowGraphics::setPosition(const Ogre::Real x,
                                   const Ogre::Real y,
                                   const Ogre::Real z)
{
    m_node->setPosition(x, y, z);
}

// ========================================================================= //