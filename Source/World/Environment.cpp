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
// File: Environment.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Environment class.
// ========================================================================= //

#include "Environment.hpp"
#include "Rendering/Ocean/Ocean.hpp"
#include "World.hpp"

// ========================================================================= //

Environment::Environment(World* world,
                         Graphics& graphics) :
m_world(world),
m_dLight(nullptr),
//m_world(nullptr), // Compiler complains 'm_world' : already initialized
m_graphics(graphics),
m_ocean(nullptr),
m_renderOcean(false),
m_sky(nullptr),
m_renderSky(false)
{
    m_world = world;
}

// ========================================================================= //

Environment::~Environment(void)
{

}

// ========================================================================= //

void Environment::init(void)
{
    // Set default ambient light to darkness.
    m_world->getSceneManager()->setAmbientLight(Ogre::ColourValue::Black);

    // Create directional light and set it to nothing.
    m_dLight = m_world->getSceneManager()->createLight("dLight");
    m_dLight->setType(Ogre::Light::LT_DIRECTIONAL);
    m_dLight->setDiffuseColour(Ogre::ColourValue::Black);
    m_dLight->setSpecularColour(Ogre::ColourValue::Black);
    m_dLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
}

// ========================================================================= //

void Environment::destroy(void)
{
    m_world->getSceneManager()->destroyLight(m_dLight);
}

// ========================================================================= //

void Environment::update(void)
{
    if (m_renderOcean){
        m_ocean->update();
    }
}

// ========================================================================= //

void Environment::loadOcean(const std::string& cfg, ActorComponentPtr actorC)
{
    m_ocean.reset(new Ocean(*m_world, m_graphics.ocean, cfg, actorC));
    m_renderOcean = true;
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void Environment::setAmbientLight(const Ogre::Real r,
                                  const Ogre::Real g,
                                  const Ogre::Real b)
{
    m_world->getSceneManager()->setAmbientLight(Ogre::ColourValue(r, g, b));
}

// ========================================================================= //

void Environment::setFog(const Ogre::FogMode mode,
                         const Ogre::Real r,
                         const Ogre::Real g,
                         const Ogre::Real b,
                         const Ogre::Real expDensity,
                         const Ogre::Real linearStart = 50.f,
                         const Ogre::Real linearEnd = 500.f)
{
    m_world->getSceneManager()->setFog(mode,
                                      Ogre::ColourValue(r, g, b),
                                      expDensity,
                                      linearStart,
                                      linearEnd);
}

// ========================================================================= //

void Environment::setOceanPosition(const Ogre::Real x,
                                   const Ogre::Real y,
                                   const Ogre::Real z)
{
    Assert(m_ocean != nullptr, "null Ocean");

    m_ocean->setPosition(x, y, z);
}

// ========================================================================= //