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
// File: Ocean.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Ocean class.
// ========================================================================= //

#include "Component/ActorComponent.hpp"
#include "Ocean.hpp"
#include "World/World.hpp"

// ========================================================================= //

Ocean::Ocean(World& world, 
             const Graphics::Setting graphicsSetting,
             const std::string& cfg,
             ActorComponentPtr actorC) :
m_hydrax(nullptr),
m_hydraxCamera(nullptr),
m_actorC(actorC),
m_graphicsSetting(graphicsSetting)
{
    switch (m_graphicsSetting){
    default:
    case Graphics::Setting::Low:

        break;

    case Graphics::Setting::High:       
        {
            // Create an Ogre::Camera for Hydrax.
            m_hydraxCamera = world.getSceneManager()->createCamera("HydraxCamera");
            m_hydraxCamera->setNearClipDistance(0.1f);
            m_hydraxCamera->setFarClipDistance(99999.f * 6.f);
            m_hydraxCamera->setAspectRatio(Ogre::Real(world.getViewport()->getActualWidth()) /
                                           Ogre::Real(world.getViewport()->getActualHeight()));

            // Initialize Hydrax object.
            m_hydrax = new Hydrax::Hydrax(world.getSceneManager(),
                                          m_hydraxCamera,
                                          world.getViewport());
            
            // Initialize Hydrax module.
            Hydrax::Module::ProjectedGrid* module =
                new Hydrax::Module::ProjectedGrid(m_hydrax,
                new Hydrax::Noise::Perlin(),
                Ogre::Plane(Ogre::Vector3(0.f, 1.f, 0.f), Ogre::Vector3(0.f, 0.f, 0.f)),
                Hydrax::MaterialManager::NM_VERTEX,
                Hydrax::Module::ProjectedGrid::Options());
            m_hydrax->setModule(static_cast<Hydrax::Module::Module*>(module));

            // Load settings from Hydrax config file.
            m_hydrax->loadCfg(cfg);

            // Create the ocean.
            m_hydrax->create();
        }
        break;
    }
}

// ========================================================================= //

Ocean::~Ocean(void)
{
    switch (m_graphicsSetting){
    default:
        break;

    case Graphics::Setting::High:
        m_hydrax->remove();
        delete m_hydrax;
        break;
    }
}

// ========================================================================= //

void Ocean::update(void)
{
    switch (m_graphicsSetting){
    default:
        break;

    case Graphics::Setting::Low:

        break;

    case Graphics::Setting::High:
        // Update Hydrax camera.
        m_hydraxCamera->setPosition(m_actorC->getPosition());
        m_hydraxCamera->setOrientation(m_actorC->getOrientation());

        m_hydrax->update(1.f / 16.f);               
        
        break;
    }
}

// ========================================================================= //