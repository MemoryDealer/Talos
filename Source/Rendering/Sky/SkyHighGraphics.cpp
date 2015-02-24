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
// File: SkyHighGraphics.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements SkyHighGraphics class.
// ========================================================================= //

#include "Component/ActorComponent.hpp"
#include "Component/CameraComponent.hpp"
#include "Entity/Entity.hpp"
#include "Rendering/Ocean/OceanHighGraphics.hpp"
#include "SkyHighGraphics.hpp"
#include "SkyPresets.hpp"
#include "World/Environment.hpp"
#include "World/World.hpp"

// ========================================================================= //

SkyHighGraphics::SkyHighGraphics(void) :
m_skyX(nullptr),
m_basicController(nullptr),
m_camera(nullptr),
m_day(true),
m_sunrise(0.f),
m_sunset(0.f),
m_graphicsSetting(Graphics::Setting::High)
{
    
}

// ========================================================================= //

SkyHighGraphics::~SkyHighGraphics(void)
{
    
}

// ========================================================================= //

void SkyHighGraphics::init(World* world,
                           const Graphics::Setting graphicsSetting,
                           const std::string& cfg)
{
    // Assign World pointer.
    m_world = world;

    m_graphicsSetting = graphicsSetting;

    // Create SkyX controller.
    m_basicController = new SkyX::BasicController();

    // Create SkyX system.
    m_skyX = new SkyX::SkyX(world->getSceneManager(), m_basicController);
    m_skyX->create();

    m_skyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(
        Ogre::Vector2(2.f, -1.f));    

    // Add a layer of clouds.
    m_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options());

    // Setup day/night cycle data.
    m_sunrise = 7.50f;
    m_sunset = 20.50f;

    world->getEnvironment()->setMoonEnabled(false);

    // Slow down the day/night cycle.
    m_skyX->setTimeMultiplier(0.01f);

    // Assign the Ogre::Camera pointer for updating.
    Assert(world->getPlayer() != nullptr, "Invalid Player object");
    m_camera = world->getPlayer()->getComponent<CameraComponent>()->
        getCamera();
    Assert(m_camera != nullptr, "SkyX initialized with invalid Player Camera");

    m_skyX->getVCloudsManager()->getVClouds()->registerCamera(m_camera);

    this->loadPreset(SkyPresets[SkyPreset::Thunderstorm2]);
}

// ========================================================================= //

void SkyHighGraphics::loadPreset(const SkyPreset& preset)
{
    m_skyX->setTimeMultiplier(preset.timeMultiplier);
    m_basicController->setTime(preset.time);
    m_basicController->setMoonPhase(preset.moonPhase);
    m_skyX->getAtmosphereManager()->setOptions(preset.atmosphereOpt);

    // Process layered clouds.
    if (preset.layeredClouds)
    {
        // Create layer cloud.
        if (m_skyX->getCloudsManager()->getCloudLayers().empty())
        {
            m_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options());
        }
    }
    else
    {
        // Remove layer cloud.
        if (!m_skyX->getCloudsManager()->getCloudLayers().empty())
        {
            m_skyX->getCloudsManager()->removeAll();
        }
    }

    m_skyX->getVCloudsManager()->setWindSpeed(preset.vcWindSpeed);
    m_skyX->getVCloudsManager()->setAutoupdate(preset.vcAutoupdate);

    SkyX::VClouds::VClouds* vclouds = m_skyX->getVCloudsManager()->getVClouds();

    vclouds->setWindDirection(preset.vcWindDir);
    vclouds->setAmbientColor(preset.vcAmbientColor);
    vclouds->setLightResponse(preset.vcLightResponse);
    vclouds->setAmbientFactors(preset.vcAmbientFactors);
    vclouds->setWheater(preset.vcWheater.x, preset.vcWheater.y, false);

    if (preset.volumetricClouds)
    {
        // Create VClouds.
        if (!m_skyX->getVCloudsManager()->isCreated())
        {
            // SkyX::MeshManager::getSkydomeRadius(...) works for both finite 
            // and infinite(=0) camera far clip distances.
            m_skyX->getVCloudsManager()->create(m_skyX->getMeshManager()->
                                               getSkydomeRadius(m_camera));
        }
    }
    else
    {
        // Remove VClouds.
        if (m_skyX->getVCloudsManager()->isCreated())
        {
            m_skyX->getVCloudsManager()->remove();
        }
    }

    vclouds->getLightningManager()->setEnabled(
        preset.vcLightnings);
    vclouds->getLightningManager()->setAverageLightningApparitionTime(
        preset.vcLightningsAT);
    vclouds->getLightningManager()->setLightningColor(
        preset.vcLightningsColor);
    vclouds->getLightningManager()->setLightningTimeMultiplier(
        preset.vcLightningsTM);

    m_skyX->update(0.f);
}

// ========================================================================= //

void SkyHighGraphics::destroy(void)
{
    delete m_skyX;
}

// ========================================================================= //

void SkyHighGraphics::update(void)
{
    // Update SkyX animation state.
    m_skyX->notifyCameraRender(m_camera);
    m_skyX->update(1.f / 16.f);
    
    // Process day/night cycle lighting.
    Ogre::Real time = this->getTime();
    if (m_day){
        // See if it is night.
        if (time > m_sunset || time < m_sunrise){
            m_day = false;
            // Set lighting to night.
            m_world->getEnvironment()->setSunEnabled(false);
            m_world->getEnvironment()->setMoonEnabled(true);
        }
        
        // Update sun direction.
        m_world->getEnvironment()->setSunDirection(
            -m_basicController->getSunDirection());
    }
    else{
        // See if it is day.
        if (time > m_sunrise && time < m_sunset){
            m_day = true;
            // Set lighting to day.
            m_world->getEnvironment()->setSunEnabled(true);
            m_world->getEnvironment()->setMoonEnabled(false);
        }

        // Update sun direction.
        // @TODO: Replace sun with moon light.
        m_world->getEnvironment()->setSunDirection(
            -m_basicController->getSunDirection());
        // Update moon direction.
        m_world->getEnvironment()->setMoonDirection(
            -m_basicController->getMoonDirection());
    }
}

// ========================================================================= //

const Ogre::Real SkyHighGraphics::calcSkydomeRadius(void) const
{
    return m_skyX->getMeshManager()->getSkydomeRadius(m_camera) * 0.5f;
}

// ========================================================================= //