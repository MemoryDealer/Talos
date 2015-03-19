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
// File: RotationComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines RotationComponent class.
// ========================================================================= //

#ifndef __ROTATIONCOMPONENT_HPP__
#define __ROTATIONCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Rotates scene nodes each frame by specified value on specified axis.
class RotationComponent : public Component
{
public:
    // Default initializes member data.
    explicit RotationComponent(void);

    // Empty destructor.
    virtual ~RotationComponent(void) override;

    // Empty.
    virtual void init(void) override;

    // Empty.
    virtual void destroy(void) override;

    // Applies all rotations.
    virtual void update(void) override;

    // Handles messages.
    virtual void message(ComponentMessage& msg) override;

    // Component functions:

    // Adds a new rotation to the list of rotations to process each frame.
    void addRotation(const Ogre::Vector3& axis,
                     const Ogre::Degree& amount,
                     const std::string& nodeName = "");

    // Assigns scene nodes to each rotation based on names.
    void setup(SceneComponentPtr sceneC);

    // === //

    struct Rotation{
        Ogre::SceneNode* node;
        Ogre::Vector3 axis;
        Ogre::Radian angle;
    };

private:
    std::vector<Rotation> m_rotations;
    std::vector<std::string> m_nodeNames;
};

// ========================================================================= //

#endif

// ========================================================================= //