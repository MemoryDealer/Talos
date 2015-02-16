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

#include "Rendering/Ocean/Ocean.hpp"
#include "Sky.hpp"
#include "World/World.hpp"

// ========================================================================= //

Sky::Sky(World& world, 
         const Graphics::Setting graphicsSetting,
         const std::string& cfg) :
m_skyX(nullptr),
m_basicController(nullptr),
m_graphicsSetting(graphicsSetting)
{
    switch (m_graphicsSetting){
    default:
        break;

    case Graphics::Setting::Low:

        break;

    case Graphics::Setting::High:
        // Setup SkyX.
        m_basicController = new SkyX::BasicController();
        m_skyX = new SkyX::SkyX(world.getSceneManager(), m_basicController);
        m_skyX->create();
        m_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options());
        world.getRoot()->addFrameListener(m_skyX);
        static_cast<Ogre::RenderWindow*>(world.getRoot()->getRenderTarget("Engine"))->addListener(m_skyX);
        //m_basicController->setTime(Ogre::Vector3(13.f, 7.f, 18.5f));
        m_skyX->setTimeMultiplier(0.005f);
        break;
    }
}

// ========================================================================= //

Sky::~Sky(void)
{
    switch (m_graphicsSetting){
    default:
        break;

    case Graphics::Setting::Low:

        break;

    case Graphics::Setting::High:
        m_skyX->remove();
        delete m_skyX;
        delete m_basicController;
        break;
    }
}

// ========================================================================= //

void Sky::update(void)
{
    switch (m_graphicsSetting){
    default:
        break;

    case Graphics::Setting::Low:

        break;

    case Graphics::Setting::High:
        /*m_basicController->update(1.f / 16.f);
        m_skyX->update(1.f / 16.f);*/
        
        break;
    }
}

// ========================================================================= //

const Ogre::Real Sky::calcSkydomeRadius(Ogre::Camera* camera) const
{
    return m_skyX->getMeshManager()->getSkydomeRadius(camera)
        * 0.5f;
}

// ========================================================================= //