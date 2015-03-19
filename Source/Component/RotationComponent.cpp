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
// File: RotationComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements RotationComponent class.
// ========================================================================= //

#include "RotationComponent.hpp"
#include "SceneComponent.hpp"

// ========================================================================= //

RotationComponent::RotationComponent(void) :
m_rotations(),
m_nodeNames()
{

}

// ========================================================================= //

RotationComponent::~RotationComponent(void)
{

}

// ========================================================================= //

void RotationComponent::init(void)
{

}

// ========================================================================= //

void RotationComponent::destroy(void)
{

}

// ========================================================================= //

void RotationComponent::update(void)
{
    for (auto& i : m_rotations){
        i.node->rotate(i.axis, i.angle);
    }
}

// ========================================================================= //

void RotationComponent::message(ComponentMessage& msg)
{

}

// ========================================================================= //

// Component functions:

// ========================================================================= //

void RotationComponent::addRotation(const Ogre::Vector3& axis,
                                    const Ogre::Real& angle,
                                    const std::string& nodeName)
{
    Rotation rotation;
    rotation.node = nullptr; // Will be assigned in setup().
    rotation.axis = axis;
    rotation.angle = Ogre::Degree(angle).valueRadians();
    m_rotations.push_back(rotation);

    m_nodeNames.push_back(nodeName);
}

// ========================================================================= //

void RotationComponent::setup(SceneComponentPtr sceneC)
{
    // Find the scene nodes in the scene component with the assigned names and
    // assign the scene node pointer for rotation.
    auto rotation = std::begin(m_rotations);
    auto name = std::begin(m_nodeNames);
    for (;
         rotation != std::end(m_rotations);
         ++rotation, ++name){
        // If the node name is empty, assign the root scene node.
        if (name->compare("") == 0){
            rotation->node = sceneC->getSceneNode();
        }
        else{
            // Find the child scene node with the name.
            Ogre::SceneNode* node = sceneC->getSceneNode();
            auto itr = node->getChildIterator();
            while(itr.hasMoreElements()){
                Ogre::SceneNode* child = 
                    static_cast<Ogre::SceneNode*>(itr.getNext());
                if (child->getName().compare(*name) == 0){
                    rotation->node = child;
                }
            }
        }

        Assert(rotation->node != nullptr, "Rotation node not found!");
    }
}

// ========================================================================= //