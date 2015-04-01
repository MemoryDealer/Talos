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

class Geom;
class SSAO;
struct QuadRenderer;

// ========================================================================= //
// Responsible for managing visual environment rendered in Ogre3D.
class Environment final
{
public:
    // Assigns Ogre::SceneManager* pointer, initializes data to nullptr.
    explicit Environment(std::shared_ptr<World> world, Graphics& graphics);

    // Empty destructor.
    ~Environment(void);

    // Sets ambient light to darkness and sets up default sun/moon lights to
    // darkness and pointing straight down.
    void init(void);

    // Destroys directional light.
    void destroy(void);

    // Disables SkyX and HydraX to allow for use in other states.
    void pause(void);

    // Re-enables SkyX and HydraX with previous settings.
    void resume(void);

    // Updates directional light, water, and sky if active.
    void update(void);

    // Allocates Ocean object according to graphics settings.
    void loadOcean(const std::string& cfg);

    // Allocates Sky object, uses config file passed in for settings.
    void loadSky(const std::string& = "");

    // Loads rendering effects such as shadows, SSAO if activated.
    void loadEffects(void);

    // Getters:

    // Returns pointer to the World that created it.
    std::shared_ptr<World> getWorld(void) const;

    // Returns pointer to active Ocean object.
    std::shared_ptr<Ocean> getOcean(void) const;

    // Returns pointer to active Sky object.
    std::shared_ptr<Sky> getSky(void) const;

    // Setters:

    // Sets colour of world's ambient light.
    void setAmbientLight(const Ogre::Real, 
                         const Ogre::Real, 
                         const Ogre::Real);

    // Sets direction of the world's sun light.
    void setSunDirection(const Ogre::Vector3&);

    // Sets direction of the world's sun light.
    void setSunDirection(const Ogre::Real, 
                         const Ogre::Real, 
                         const Ogre::Real);

    // Sets colour of world's sun light.
    void setSunColour(const Ogre::Real, 
                      const Ogre::Real, 
                      const Ogre::Real);

    // Enables the sun directional light to light the world if true, set to
    // darkness if false.
    void setSunEnabled(const bool);

    // Sets direction of the world's moon light.
    void setMoonDirection(const Ogre::Vector3&);

    // Sets direction of the world's moon light.
    void setMoonDirection(const Ogre::Real, 
                          const Ogre::Real, 
                          const Ogre::Real);

    // Sets colour of world's moon light.
    void setMoonColour(const Ogre::Real, 
                       const Ogre::Real, 
                       const Ogre::Real);

    // Enables the moon directional light to light the world if true, set to
    // darkness if false.
    void setMoonEnabled(const bool);

    // Sets fog mode, colour, along with any parameters needed.
    void setFog(const Ogre::FogMode mode,
                const Ogre::Real r,
                const Ogre::Real g,
                const Ogre::Real b,
                const Ogre::Real expDensity,
                const Ogre::Real linearStart,
                const Ogre::Real linearEnd);

    // Sets origin of Ocean.
    void setOceanPosition(const Ogre::Real, 
                          const Ogre::Real, 
                          const Ogre::Real);

private:
    // Directional lights.
    Ogre::Light* m_sun; 
    Ogre::ColourValue m_sunColour;
    Ogre::Light* m_moon;
    Ogre::ColourValue m_moonColour;

    std::shared_ptr<World> m_world;
    Graphics m_graphics;

    // Water and Sky.
    std::shared_ptr<Ocean> m_ocean;
    std::shared_ptr<Sky> m_sky;
    bool m_renderOcean, m_renderSky;
    std::string m_oceanCfg, m_skyCfg;

    // SSAO.
    std::shared_ptr<SSAO> m_ssao;
    std::shared_ptr<Geom> m_geom;
    std::shared_ptr<QuadRenderer> m_qr;
};

// ========================================================================= //

// Getters:

inline std::shared_ptr<World> Environment::getWorld(void) const{
    return m_world;
}


inline std::shared_ptr<Ocean> Environment::getOcean(void) const{
    return m_ocean;
}


inline std::shared_ptr<Sky> Environment::getSky(void) const{
    return m_sky;
}

// ========================================================================= //

#endif

// ========================================================================= //