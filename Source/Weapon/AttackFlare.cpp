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
// File: AttackFlare.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements AttackFlare class.
// ========================================================================= //

#include "AttackFlare.hpp"
#include "World/World.hpp"

// ========================================================================= //

AttackFlare::AttackFlare(void) :
m_world(nullptr),
m_node(nullptr),
m_bbSet(nullptr),
m_bb(nullptr),
m_light(nullptr),
m_timer(),
m_scale(1.f),
m_life(0.f),
m_rate(0.f),
m_max(0.f),
m_active(false)
{

}

// ========================================================================= //

AttackFlare::~AttackFlare(void)
{

}

// ========================================================================= //

void AttackFlare::init(std::shared_ptr<World> world, 
                       Ogre::SceneNode* parent,
                       const std::string& cfg)
{
    m_world = world;

    Ogre::SceneManager* mgr = world->getSceneManager();

    Ogre::ColourValue colour = Ogre::ColourValue(0.f, 0.f, 10.f);

    // Setup billboard.
    m_bbSet = mgr->createBillboardSet(1);
    m_bbSet->setMaterialName("AttackFlare1");
    m_bb = m_bbSet->createBillboard(Ogre::Vector3::ZERO);
    m_bbSet->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_9);

    // Create light flash.
    m_light = mgr->createLight();
    m_light->setType(Ogre::Light::LT_POINT);
    m_light->setCastShadows(false);
    m_light->setDiffuseColour(colour);
    m_light->setSpecularColour(colour);
    m_light->setAttenuation(25.f, 1.f, 1.f, 0.f);

    // Setup life data.
    m_life = 50.f;
    m_rate = 0.6f;
    m_max = 40.f;

    // Attach all to scene node.
    m_node = parent->createChildSceneNode();
    m_node->attachObject(m_bbSet);
    m_node->translate(0.f, 0.1f, 0.f);

    if (m_light){
        m_node->attachObject(m_light);
    }    

    // Scale the node.
    m_scale = 0.1f;
    m_node->setScale(m_scale, m_scale, m_scale);

    // Set to invisible initially.
    m_node->setVisible(false);
}

// ========================================================================= //

void AttackFlare::activate(void)
{
    if (!m_active){
        // Make this flare visible.
        m_node->setVisible(true);
        m_node->setScale(m_scale, m_scale, m_scale);

        m_active = true;
        m_timer.reset();
    }
}

// ========================================================================= //

void AttackFlare::update(void)
{
    if (m_active){
        if (m_timer.getMillisecondsCPU() > m_life){
            m_active = false;
            m_node->setVisible(false);
            return;
        }

        Ogre::Vector3 growth = 
            Ogre::Vector3(m_scale, m_scale, m_scale) + m_rate;
        m_node->scale(growth);
    }
}

// ========================================================================= //