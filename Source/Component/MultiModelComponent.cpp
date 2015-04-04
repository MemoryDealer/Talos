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
// File: MultiModelComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements MultiModelComponent class.
// ========================================================================= //

#include "Loader/DotSceneLoader.hpp"
#include "MultiModelComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

MultiModelComponent::MultiModelComponent(void) :
m_sceneFile(),
m_mat()
{

}

// ========================================================================= //

MultiModelComponent::~MultiModelComponent(void)
{

}

// ========================================================================= //

void MultiModelComponent::destroy(void)
{

}

// ========================================================================= //

void MultiModelComponent::message(ComponentMessage& msg)
{

}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void MultiModelComponent::setMesh(const std::string& file,
                                  const std::string& mat)
{
    m_sceneFile = file;
    m_mat = mat;
}

// ========================================================================= //

void MultiModelComponent::setup(Ogre::SceneNode* attachNode)
{
    DotSceneLoader loader;
    loader.parseDotScene(m_sceneFile,
                         "General",
                         this->getWorld()->getSceneManager(),
                         attachNode);

    m_sceneFile.clear();
}

// ========================================================================= //