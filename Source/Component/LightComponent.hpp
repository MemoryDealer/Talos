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
    enum class Type{
        Point = 0,
        Spotlight
    };

    // Sets type.
    explicit LightComponent(void);

    // Empty destructor.
    virtual ~LightComponent(void) override;

    // Creates Ogre::Light and sets to point light by default.
    virtual void init(void) override;

    // Destroys Ogre::Light using World's Ogre::SceneManager.
    virtual void destroy(void) override;

    // Empty.
    virtual void update(void) override;

    // Handles activation messages.
    virtual void message(ComponentMessage& msg) override;

    // Getters:

    // Returns type of light.
    const Type& getType(void) const;

    // Returns pointer to internal Ogre::Light.
    Ogre::Light* getLight(void) const;

    // Setters:

    // Sets type of the light.
    void setType(const Type& type);

    // Sets colour of light.
    void setColour(const Ogre::Real r, 
                   const Ogre::Real g, 
                   const Ogre::Real b);

    // Sets range of light using linear and quadratic values internally.
    void setRange(const Ogre::Real range);

    // Turns light on if true.
    void setEnabled(const bool enabled);

private:
    Ogre::Light* m_light;
    Ogre::Real m_intensity;
    Type m_type;
};

// ========================================================================= //

#endif

// ========================================================================= //