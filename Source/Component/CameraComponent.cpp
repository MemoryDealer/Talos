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
// File: CameraComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements CameraComponent class.
// ========================================================================= //

#include "CameraComponent.hpp"
#include "ComponentMessage.hpp"
#include "SceneComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

CameraComponent::CameraComponent(void) :
Component(),
m_camera(nullptr)
{
    
}

// ========================================================================= //

CameraComponent::~CameraComponent(void)
{

}

// ========================================================================= //

void CameraComponent::init(World& world)
{
    // Create the camera object.
    m_camera = world.getSceneManager()->createCamera("PlayerCam");
    m_camera->setNearClipDistance(0.1f);
    m_camera->setFarClipDistance(0.f);

    // Setup viewport aspect ratio and assign camera to viewport.
    Ogre::Viewport* viewport = world.getViewport();
    m_camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) /
                             Ogre::Real(viewport->getActualHeight()));
    viewport->setCamera(m_camera);
}

// ========================================================================= //

void CameraComponent::destroy(World& world)
{
    world.getSceneManager()->destroyCamera(m_camera);
}

// ========================================================================= //

void CameraComponent::update(World& world)
{
    
}

// ========================================================================= //

void CameraComponent::message(ComponentMessage& msg)
{

}

// ========================================================================= //