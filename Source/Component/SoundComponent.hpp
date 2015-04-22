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
// File: SoundComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines SoundComponent class.
// ========================================================================= //

#ifndef __SOUNDCOMPONENT_HPP__
#define __SOUNDCOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Associates a 3D sound with an entity.
class SoundComponent final : public Component
{
public:
    explicit SoundComponent(void);

    virtual ~SoundComponent(void) override;

    virtual void destroy(void) override;

    virtual void update(void) override;

    virtual void message(ComponentMessage& msg) override;

    // Component functions:

    void addSound(const std::string& file, const bool looped = true);

    // Sets internal scene node pointer to positional updates.
    void setSceneNode(Ogre::SceneNode* node);

private:
    irrklang::ISound* m_sound;
    Ogre::SceneNode* m_node;
    bool m_looped;
};

// ========================================================================= //

#endif

// ========================================================================= //