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
// File: Sky.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Sky class.
// ========================================================================= //

#include "Component/CameraComponent.hpp"
#include "Entity/Entity.hpp"
#include "SkyHighGraphics.hpp"
#include "World/World.hpp"

// ========================================================================= //

SkyHighGraphics::SkyHighGraphics(void) :
m_skyX(nullptr),
m_basicController(nullptr),
m_camera(nullptr),
m_graphicsSetting(Graphics::Setting::High)
{
    
}

// ========================================================================= //

SkyHighGraphics::~SkyHighGraphics(void)
{
    
}

// ========================================================================= //

void SkyHighGraphics::init(World& world,
                           const Graphics::Setting,
                           const std::string&)
{
    // Create SkyX controller.
    m_basicController = new SkyX::BasicController();

    // Create SkyX system.
    m_skyX = new SkyX::SkyX(world.getSceneManager(), m_basicController);
    m_skyX->create();

    // Add a layer of clouds.
    m_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options());

    // Slow down the day/night cycle.
    m_skyX->setTimeMultiplier(0.1f);

    // Assign the Ogre::Camera pointer for updating.
    Assert(world.getPlayer() != nullptr, "Invalid Player object");
    m_camera = static_cast<CameraComponentPtr>(
        world.getPlayer()->getComponentPtr(
        Component::Type::Camera))->getCamera();
    Assert(m_camera != nullptr, "SkyX initialized with invalid Player Camera");
}

// ========================================================================= //

void SkyHighGraphics::destroy(void)
{
    m_skyX->remove();
    delete m_skyX;
    delete m_basicController;
}

// ========================================================================= //

void SkyHighGraphics::update(void)
{
    m_skyX->update(1.f / 16.f);
    m_skyX->notifyCameraRender(m_camera);
}

// ========================================================================= //

const Ogre::Real SkyHighGraphics::calcSkydomeRadius(void) const
{
    return m_skyX->getMeshManager()->getSkydomeRadius(m_camera) * 0.5f;
}

// ========================================================================= //