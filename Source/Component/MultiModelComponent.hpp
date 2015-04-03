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
// File: MultiModelComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines MultiModelComponent class.
// ========================================================================= //

#ifndef __MULTIMODELCOMPONENT_HPP__
#define __MULTIMODELCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// A 3D model consisting of multiple .mesh files, attached to a single scene
// node for efficient control.
class MultiModelComponent : public Component
{
public:
    // Default initializes member data.
    explicit MultiModelComponent(void);

    // Empty destructor.
    virtual ~MultiModelComponent(void) override;

    virtual void destroy(void) override;

    virtual void message(ComponentMessage& msg) override;

    // Component functions:

    // Saves scene file name for later setup.
    virtual void setMesh(const std::string& file,
                         const std::string& mat = "");

    // Parses .scene file, attachs all entities to attachNode.
    void setup(Ogre::SceneNode* attachNode);

private:
    std::string m_sceneFile;
};

// ========================================================================= //

#endif

// ========================================================================= //