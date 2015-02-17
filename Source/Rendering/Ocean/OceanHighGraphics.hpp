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
// File: OceanHighGraphics.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines OceanHighGraphics class.
// ========================================================================= //

#ifndef __OCEANHIGHGRAPHICS_HPP__
#define __OCEANHIGHGRAPHICS_HPP__

// ========================================================================= //

#include "Ocean.hpp"

// ========================================================================= //

class ActorComponent;
class World;

// ========================================================================= //
// Renders high-detail ocean using Hydrax Ogre3D plugin (Perlin or FFT noise).
class OceanHighGraphics final : public Ocean
{
public:
    // Default initializes member data.
    explicit OceanHighGraphics(void);

    // Emtpy destructor.
    virtual ~OceanHighGraphics(void) override;

    // Create Hydrax
    void init(World*, const std::string&, const Graphics::Setting);

    // Removes Hydrax and clears the memory it allocated.
    virtual void destroy(void) override;

    // Updates Hydrax camera position/orientation and Hydrax animation.
    virtual void update(void) override;

    // Setters:

    // Sets origin of Hydrax ocean.
    virtual void setPosition(const Ogre::Real, 
                             const Ogre::Real, 
                             const Ogre::Real) override;

    // Sets position of where the sun should be, affecting light reflections
    // and underwater god rays.
    virtual void setSunPosition(const Ogre::Vector3&) override;

    // Sets colour of sun as seen in water reflection and underwater.
    virtual void setSunColour(const Ogre::Vector3&) override;

    // Sets colour of sun as seen in water reflection and underwater.
    virtual void setSunColour(const Ogre::ColourValue&) override;

    // Sets sun effects to visible if true.
    virtual void setSunEnabled(const bool) override;

private:
    Hydrax::Hydrax* m_hydrax;
    Ogre::Camera* m_hydraxCamera;

    // The actor which controls the camera. Needed for updating Hydrax camera.
    ActorComponent* m_actorC;
    Graphics::Setting m_graphicsSetting;
};

// ========================================================================= //

// Setters:

inline void OceanHighGraphics::setPosition(const Ogre::Real x,
                                           const Ogre::Real y,
                                           const Ogre::Real z){
    m_hydrax->setPosition(Ogre::Vector3(x, y, z));
}

inline void OceanHighGraphics::setSunPosition(const Ogre::Vector3& pos){
    m_hydrax->setSunPosition(pos);
}

inline void OceanHighGraphics::setSunColour(const Ogre::Vector3& colour){
    m_hydrax->setSunColor(colour);
}

inline void OceanHighGraphics::setSunColour(const Ogre::ColourValue& colour){
    m_hydrax->setSunColor(Ogre::Vector3(colour.r, colour.g, colour.b));
}

inline void OceanHighGraphics::setSunEnabled(const bool enabled){
    
}

// ========================================================================= //

#endif

// ========================================================================= //