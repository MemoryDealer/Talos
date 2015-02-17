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
// File: OceanHighGraphics.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements OceanHighGraphics class.
// ========================================================================= //

#include "Component/ActorComponent.hpp"
#include "Entity/Entity.hpp"
#include "OceanHighGraphics.hpp"
#include "World/World.hpp"

// ========================================================================= //

OceanHighGraphics::OceanHighGraphics(void) :
m_hydrax(nullptr),
m_hydraxCamera(nullptr),
m_actorC(nullptr),
m_graphicsSetting(Graphics::Setting::High)
{
    
}

// ========================================================================= //

OceanHighGraphics::~OceanHighGraphics(void)
{
    
}

// ========================================================================= //

void OceanHighGraphics::init(World* world,
                             const std::string& cfg,
                             const Graphics::Setting graphicsSetting)
{
    // Assign ActorComponentPtr for update procedure.
    m_actorC = world->getPlayer()->getActorComponent();

    m_graphicsSetting = graphicsSetting;

    // Create an Ogre::Camera for Hydrax. Hydrax cannot operate correctly with
    // a camera that's attached to a scene node, so we must create a new one.
    m_hydraxCamera = world->getSceneManager()->createCamera("HydraxCamera");
    m_hydraxCamera->setNearClipDistance(0.1f);
    m_hydraxCamera->setFarClipDistance(99999.f * 6.f);
    m_hydraxCamera->setAspectRatio(
        Ogre::Real(world->getViewport()->getActualWidth()) /
        Ogre::Real(world->getViewport()->getActualHeight()));

    // Initialize Hydrax object.
    m_hydrax = new Hydrax::Hydrax(world->getSceneManager(),
                                  m_hydraxCamera,
                                  world->getViewport());

    // Initialize Hydrax module.
    Hydrax::Module::ProjectedGrid* module =
        new Hydrax::Module::ProjectedGrid(m_hydrax,
        new Hydrax::Noise::Perlin(),
        Ogre::Plane(Ogre::Vector3(0.f, 1.f, 0.f), 
            Ogre::Vector3(0.f, 0.f, 0.f)),
        Hydrax::MaterialManager::NM_VERTEX,
        Hydrax::Module::ProjectedGrid::Options());
    m_hydrax->setModule(static_cast<Hydrax::Module::Module*>(module));

    // Load settings from Hydrax config file.
    m_hydrax->loadCfg(cfg);

    // Create the ocean.
    m_hydrax->create();
}

// ========================================================================= //

void OceanHighGraphics::destroy(void)
{
    m_hydrax->remove();
    delete m_hydrax;
}

// ========================================================================= //

void OceanHighGraphics::update(void)
{
    // Update Hydrax camera.
    m_hydraxCamera->setPosition(m_actorC->getPosition());
    m_hydraxCamera->setOrientation(m_actorC->getOrientation());

    // Update Hydrax animation.
    m_hydrax->update(1.f / 16.f);
}

// ========================================================================= //