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
// File: SkyHighGraphics.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines SkyHighGraphics class.
// ========================================================================= //

#ifndef __SKYHIGHGRAPHICS_HPP__
#define __SKYHIGHGRAPHICS_HPP__

// ========================================================================= //

#include "Rendering/Sky/SkyX/SkyX.h"
#include "Sky.hpp"

// ========================================================================= //
// Renders high-detail sky with cloud layers, sun/moon, day & night cycles,
// and weather such as thunderstorms and lighting.
class SkyHighGraphics final : public Sky
{
public:
    // Default initializes member data.
    explicit SkyHighGraphics(void);

    // Empty destructor.
    virtual ~SkyHighGraphics(void) override;

    // Creates SkyX object with default settings.
    void init(std::shared_ptr<World> world,
              const std::string& cfg,
              const Graphics::Setting setting);

    // Applies a SkyPreset's data to SkyX.
    virtual void loadPreset(const SkyPreset&) override;

    // Removes SkyX and frees the allocated memory.
    virtual void destroy(void) override;

    // Update SkyX animation state and day/night cycle lighting.
    virtual void update(void) override;

    // Getters:

    // Returns the time of day on a [0, 24] scale. Sunrise is at 7.50f, and
    // sunset is at ~20.50f.
    virtual const Ogre::Real getTime(void) const override;

    // Gets the direction of the sun in the sky relative to player.
    virtual const Ogre::Vector3 getSunDirection(void) const override;

    // Gets the direction of the moon in the sky relative to player.
    virtual const Ogre::Vector3 getMoonDirection(void) const override;

    // Returns SkyX sky dome radius.
    virtual const Ogre::Real calcSkydomeRadius(void) const override;

private:
    SkyX::SkyX* m_skyX;
    SkyX::BasicController* m_basicController;
    Ogre::Camera* m_camera;
    bool m_day;
    Ogre::Real m_sunrise, m_sunset;
    
    std::shared_ptr<World> m_world;
    Graphics::Setting m_graphicsSetting;
};

// ========================================================================= //

inline const Ogre::Real SkyHighGraphics::getTime(void) const{
    return m_basicController->getTime().x;
}

inline const Ogre::Vector3 SkyHighGraphics::getSunDirection(void) const{
    return m_basicController->getSunDirection();
}

inline const Ogre::Vector3 SkyHighGraphics::getMoonDirection(void) const{
    return m_basicController->getMoonDirection();
}

// ========================================================================= //

#endif

// ========================================================================= //