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
// File: PositionComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements PositionComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

SceneComponent::SceneComponent(void) :
Component(),
m_node(nullptr)
{
    
}

// ========================================================================= //

SceneComponent::~SceneComponent(void)
{

}

// ========================================================================= //

void SceneComponent::init(World& world)
{
    Assert(m_node == nullptr, "init() called more than once!");

    m_node = world.getSceneManager()->getRootSceneNode()->
        createChildSceneNode();
}

// ========================================================================= //

void SceneComponent::destroy(World& world)
{
    world.getSceneManager()->destroySceneNode(m_node);
}

// ========================================================================= //

void SceneComponent::update(World&)
{
    
}

// ========================================================================= //

void SceneComponent::message(const ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::Translate:
        m_node->translate(boost::get<Ogre::Vector3>(msg.data));
        break;
    }
}

// ========================================================================= //

void SceneComponent::onComponentAttached(ComponentPtr component)
{
    
}

// ========================================================================= //

void SceneComponent::attachCamera(Ogre::Camera* camera)
{
    m_node->attachObject(camera);
}

// ========================================================================= //

const Ogre::Vector3 SceneComponent::getPosition(void) const
{
    return m_node->getPosition();
}

// ========================================================================= //

const Ogre::Quaternion SceneComponent::getOrientation(void) const
{
    return m_node->getOrientation();
}

// ========================================================================= //

void SceneComponent::setPosition(const Ogre::Vector3& pos)
{
    m_node->setPosition(pos);
}

// ========================================================================= //

void SceneComponent::setPosition(const Ogre::Real x, 
                                 const Ogre::Real y, 
                                 const Ogre::Real z)
{
    m_node->setPosition(x, y, z);
}

// ========================================================================= //

void SceneComponent::setOrientation(const Ogre::Quaternion& orientation)
{
    m_node->setOrientation(orientation);
}

// ========================================================================= //

void SceneComponent::setOrientation(const Ogre::Real w, 
                                    const Ogre::Real x, 
                                    const Ogre::Real y,
                                    const Ogre::Real z)
{
    m_node->setOrientation(w, x, y, z);
}

// ========================================================================= //