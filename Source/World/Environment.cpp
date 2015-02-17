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
#include "Entity/Entity.hpp"
#include "Environment.hpp"
#include "Rendering/Ocean/Ocean.hpp"
#include "Rendering/Sky/Sky.hpp"
#include "World.hpp"

// ========================================================================= //

Environment::Environment(World* world,
                         Graphics& graphics) :
m_world(world),
m_sun(nullptr),
m_moon(nullptr),
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
        Ogre::Real time = m_sky->getTime();
        // If it's daytime.
        if (time > 7.50f && time < 20.50f){
            if (m_sun->isVisible() == false){
                m_sun->setVisible(true);
                m_ocean->setSunEnabled(true);
            }
            if (m_moon->isVisible() == true){
                m_moon->setVisible(false);
            }
            m_sun->setDirection(-m_sky->getSunDirection());

            // Update sky-dependent Ocean properties.
            if (m_renderOcean){

                Ogre::Vector3 sunPosition =
                    m_world->getPlayer()->getActorComponent()->getPosition() +
                    m_sun->getDirection() *
                    m_sky->calcSkydomeRadius(
                    m_world->getPlayer()->getActorComponent()->getCamera());
                m_ocean->setSunPosition(-sunPosition);
            }
        }
        // Otherwise nighttime.
        else{
            if (m_moon->isVisible() == false){
                m_moon->setVisible(true);
            }
            if (m_sun->isVisible() == true){
                m_sun->setVisible(false);
                if (m_renderOcean){
                    m_ocean->setSunEnabled(false);
                    m_ocean->setSunPosition(Ogre::Vector3::ZERO);
                }
            }
            m_moon->setDirection(-m_sky->getMoonDirection());
        }
    }
}

// ========================================================================= //

void Environment::loadOcean(const std::string& cfg)
{
    m_ocean.reset(new Ocean(*m_world, 
        m_graphics.ocean, 
        cfg, 
        m_world->getPlayer()->getActorComponent()));
    m_renderOcean = true;
}

// ========================================================================= //

void Environment::loadSky(const std::string& cfg)
{
    m_sky.reset(new Sky(*m_world, m_graphics.sky, cfg));
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

void Environment::setSunDirection(const Ogre::Real x,
                                  const Ogre::Real y,
                                  const Ogre::Real z)
{
    m_sun->setDirection(x, y, z);
}

// ========================================================================= //

void Environment::setSunColour(const Ogre::Real r,
                               const Ogre::Real g,
                               const Ogre::Real b)
{
    m_sun->setDiffuseColour(r, g, b);
    m_sun->setSpecularColour(r, g, b);
    if (m_renderOcean){
        m_ocean->setSunColour(Ogre::Vector3(r, g, b));
    }
}

// ========================================================================= //

void Environment::setMoonDirection(const Ogre::Real x,
                                   const Ogre::Real y,
                                   const Ogre::Real z)
{
    m_moon->setDirection(x, y, z);
}

// ========================================================================= //

void Environment::setMoonColour(const Ogre::Real r,
                                const Ogre::Real g,
                                const Ogre::Real b)
{
    m_moon->setDiffuseColour(r, g, b);
    m_moon->setSpecularColour(r, g, b);
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