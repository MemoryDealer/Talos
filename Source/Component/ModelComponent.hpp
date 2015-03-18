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
// File: ModelComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ModelComponent class.
// ========================================================================= //

#ifndef __MODELCOMPONENT_HPP__
#define __MODELCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Needed information to render a 3D model in the game world.
class ModelComponent : public Component
{
public:
    // Default initializes member data, stores mesh filename.
    explicit ModelComponent(void);

    // Empty destructor.
    virtual ~ModelComponent(void) override;

    // Empty.
    virtual void init(void) override;

    // Detaches Ogre::Entity from parent scene node. Sets to null.
    virtual void destroy(void) override;

    // Empty.
    virtual void update(void) override;

    // Empty.
    virtual void message(ComponentMessage& msg) override;

    // Loads Ogre::Entity with mesh and material.
    virtual void setMesh(const std::string& mesh,
                         const std::string& mat = "");

    // Getters:

    // Returns internal Ogre::Entity pointer.
    Ogre::Entity* getOgreEntity(void) const;

    // Setters:

    // Calls Ogre::Entity::setMaterialName() on internal Ogre::Entity.
    void setMaterialName(const std::string&);

private:
    Ogre::Entity* m_entity;
};

// ========================================================================= //

#endif

// ========================================================================= //