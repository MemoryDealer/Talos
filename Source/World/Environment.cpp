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

#include "Component/ActorComponent.hpp"
#include "Component/CameraComponent.hpp"
#include "Entity/Entity.hpp"
#include "Environment.hpp"
#include "Rendering/Ocean/OceanHighGraphics.hpp"
#include "Rendering/Ocean/OceanLowGraphics.hpp"
#include "Rendering/Sky/SkyHighGraphics.hpp"
#include "World.hpp"

// ========================================================================= //

Environment::Environment(World* world,
                         Graphics& graphics) :
m_world(world),
m_sun(nullptr),
m_sunColour(Ogre::ColourValue::Black),
m_moon(nullptr),
m_moonColour(Ogre::ColourValue::Black),
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

    // Create sun and moon light and set them to nothing.
    m_sun = m_world->getSceneManager()->createLight("Sun");
    m_sun->setType(Ogre::Light::LT_DIRECTIONAL);
    m_sun->setDiffuseColour(Ogre::ColourValue::Black);
    m_sun->setSpecularColour(Ogre::ColourValue::Black);
    m_sun->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

    m_moon = m_world->getSceneManager()->createLight("Moon");
    m_moon->setType(Ogre::Light::LT_DIRECTIONAL);
    m_moon->setDiffuseColour(Ogre::ColourValue::Black);
    m_moon->setSpecularColour(Ogre::ColourValue::Black);
    m_moon->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

    // Shadows.
    m_world->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
}

// ========================================================================= //

void Environment::destroy(void)
{
    if (m_ocean != nullptr){
        m_ocean->destroy();
    }
    if (m_sky != nullptr){
        m_sky->destroy();
    }

    m_world->getSceneManager()->destroyLight(m_sun);
    m_world->getSceneManager()->destroyLight(m_moon);
}

// ========================================================================= //

void Environment::update(void)
{
    // Update Ocean.
    if (m_renderOcean){
        m_ocean->update();        
    }

    // Update Sky.
    if (m_renderSky){
        m_sky->update();
    }
}

// ========================================================================= //

void Environment::loadOcean(const std::string& cfg)
{
    switch (m_graphics.ocean){
    default:
    case Graphics::Setting::Low:
        m_ocean.reset(new OceanLowGraphics());
        static_cast<OceanLowGraphics*>(m_ocean.get())->init(m_world,
                                                            cfg);
        return;

    case Graphics::Setting::High:
        m_ocean.reset(new OceanHighGraphics());
        static_cast<OceanHighGraphics*>(m_ocean.get())->init(m_world, 
                                                             cfg, 
                                                             m_graphics.ocean);
        break;
    }

    m_renderOcean = true;
}

// ========================================================================= //

void Environment::loadSky(const std::string& cfg)
{
    switch (m_graphics.sky){
    default:
    case Graphics::Setting::Low:

        return;

    case Graphics::Setting::High:
        m_sky.reset(new SkyHighGraphics());
        static_cast<SkyHighGraphics*>(m_sky.get())->init(m_world,
                                                         m_graphics.sky,
                                                         cfg);
        break;
    }

    m_renderSky = true;
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

void Environment::setSunDirection(const Ogre::Vector3& dir)
{
    m_sun->setDirection(dir);

    if (m_renderOcean){
        // Set the Ocean's sun position relative to player.
        m_ocean->setSunPosition(
            m_world->getMainCamera()->getCamera()->getDerivedPosition() -
            dir *
            m_sky->calcSkydomeRadius());
    }
}

// ========================================================================= //

void Environment::setSunDirection(const Ogre::Real x,
                                  const Ogre::Real y,
                                  const Ogre::Real z)
{
    this->setSunDirection(Ogre::Vector3(x, y, z));
}

// ========================================================================= //

void Environment::setSunColour(const Ogre::Real r,
                               const Ogre::Real g,
                               const Ogre::Real b)
{
    m_sunColour = Ogre::ColourValue(r, g, b);
    m_sun->setDiffuseColour(m_sunColour);
    m_sun->setSpecularColour(m_sunColour);

    // Set Ocean's sun colour if activated.
    if (m_renderOcean){
        m_ocean->setSunColour(Ogre::Vector3(r, g, b));
    }
}

// ========================================================================= //

void Environment::setSunEnabled(const bool enabled)
{
    /*m_sun->setDiffuseColour((enabled == true) ? m_sunColour :
                            Ogre::ColourValue::Black);
    m_sun->setSpecularColour((enabled == true) ? m_sunColour :
                             Ogre::ColourValue::Black);*/
    m_sun->setVisible(enabled);

    // Update Ocean's sun colour.
    if (m_renderOcean){
        m_ocean->setSunEnabled(enabled);
    }
}

// ========================================================================= //

void Environment::setMoonDirection(const Ogre::Vector3& dir)
{
    m_moon->setDirection(dir);
}

// ========================================================================= //

void Environment::setMoonDirection(const Ogre::Real x,
                                   const Ogre::Real y,
                                   const Ogre::Real z)
{
    this->setMoonDirection(Ogre::Vector3(x, y, z));
}

// ========================================================================= //

void Environment::setMoonColour(const Ogre::Real r,
                                const Ogre::Real g,
                                const Ogre::Real b)
{
    m_moonColour = Ogre::ColourValue(r, g, b);
    m_moon->setDiffuseColour(m_moonColour);
    m_moon->setSpecularColour(m_moonColour);
}

// ========================================================================= //

void Environment::setMoonEnabled(const bool enabled)
{
    /*m_moon->setDiffuseColour((enabled == true) ? m_moonColour :
                             Ogre::ColourValue::Black);
    m_moon->setSpecularColour((enabled == true) ? m_moonColour :
                              Ogre::ColourValue::Black);*/
    m_moon->setVisible(enabled);
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
    m_ocean->setPosition(x, y, z);
}

// ========================================================================= //