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
// File: LightComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines LightComponent class.
// ========================================================================= //

#ifndef __LIGHTCOMPONENT_HPP__
#define __LIGHTCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Lights up objects in the World within its range. Either a point light or a
// spotlight.
class LightComponent : public Component
{
public:
    enum Type{
        POINT = 0,
        SPOTLIGHT
    };

    // Sets type.
    explicit LightComponent(void);

    // Empty destructor.
    virtual ~LightComponent(void) override;

    // Empty.
    virtual void init(World&) override;

    // Destroys Ogre::Light using World's Ogre::SceneManager.
    virtual void destroy(World&) override;

    // Empty.
    virtual void update(World&) override;

    // Empty.
    virtual void message(ComponentMessage&) override;

    // Light functions:

    // Sets type of the light.
    void setType(const Type);

    // Sets colour of light.
    void setColour(const Ogre::Real, const Ogre::Real, const Ogre::Real);

    // Sets range of light using linear and quadratic values internally.
    void setRange(const Ogre::Real);

    

    // Getters:

    // Returns pointer to internal Ogre::Light.
    Ogre::Light* getLight(void) const;

private:
    Ogre::Light* m_light;
    Ogre::Real m_intensity;
};

// ========================================================================= //

// Getters:

inline Ogre::Light* LightComponent::getLight(void) const{
    return m_light;
}

// ========================================================================= //

#endif

// ========================================================================= //