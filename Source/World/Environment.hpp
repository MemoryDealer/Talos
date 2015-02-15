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
// File: Environment.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Environment class.
// ========================================================================= //

#ifndef __ENVIRONMENT_HPP__
#define __ENVIRONMENT_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

class ActorComponent;
class Ocean;
class Sky;
class World;

typedef ActorComponent* ActorComponentPtr;

// ========================================================================= //
// Responsible for managing visual environment rendered in Ogre3D.
class Environment final
{
public:
    // Assigns Ogre::SceneManager* pointer, initializes data to nullptr.
    explicit Environment(World*, Graphics&);

    // Empty destructor.
    ~Environment(void);

    // Sets ambient light to darkness and sets up default directional light to
    // darkness and pointing straight down.
    void init(void);

    // Destroys directional light.
    void destroy(void);

    // Updates directional light, water, and sky if active.
    void update(void);

    // Allocates Ocean object according to graphics settings.
    void loadOcean(const std::string&, ActorComponentPtr = nullptr);

    // Getters:

    // Setters:

    // Sets direction of the world's directional light.
    void setLightDirection(const Ogre::Real, const Ogre::Real, const Ogre::Real);

    // Sets colour of world's directional light.
    void setLightColour(const Ogre::Real, const Ogre::Real, const Ogre::Real);

    // Sets colour of world's ambient light.
    void setAmbientLight(const Ogre::Real, const Ogre::Real, const Ogre::Real);

    // Sets fog mode, colour, along with any parameters needed.
    void setFog(const Ogre::FogMode mode,
                const Ogre::Real r,
                const Ogre::Real g,
                const Ogre::Real b,
                const Ogre::Real expDensity,
                const Ogre::Real linearStart,
                const Ogre::Real linearEnd);

    // Sets origin of Ocean.
    void setOceanPosition(const Ogre::Real, const Ogre::Real, const Ogre::Real);

private:
    Ogre::SceneManager* m_scene;
    Ogre::Light* m_dLight; // Directional light.

    World* m_world;
    Graphics m_graphics;

    // Water and Sky.
    std::shared_ptr<Ocean> m_ocean;
    bool m_renderOcean;
    std::shared_ptr<Sky> m_sky;
    bool m_renderSky;
};

// ========================================================================= //

inline void Environment::setLightDirection(const Ogre::Real x,
                                           const Ogre::Real y,
                                           const Ogre::Real z){
    m_dLight->setDirection(x, y, z);
}

inline void Environment::setLightColour(const Ogre::Real r,
                                        const Ogre::Real g,
                                        const Ogre::Real b){
    m_dLight->setDiffuseColour(r, g, b);
    m_dLight->setSpecularColour(r, g, b);
}

// ========================================================================= //

#endif

// ========================================================================= //