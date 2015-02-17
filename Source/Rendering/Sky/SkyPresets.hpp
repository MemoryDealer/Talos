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
// File: SkyPresets.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines SkyPreset struct and SkyPresets for use by SkyHighGraphics.
// ========================================================================= //

#ifndef __SKYPRESETS_HPP__
#define __SKYPRESETS_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

struct SkyPreset
{
    enum{
        Sunset = 0,
        Clear,
        Thunderstorm1,
        Thunderstorm2,
        Desert,
        Night
    };

    // Skydome + vol. clouds + lightning settings.
    SkyPreset(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp, 
    const SkyX::AtmosphereManager::Options& atmOpt,
    const bool& lc, const bool& vc, const Ogre::Real& vcws, const bool& vcauto, 
    const Ogre::Radian& vcwd,
    const Ogre::Vector3& vcac, const Ogre::Vector4& vclr, const Ogre::Vector4& 
    vcaf, const Ogre::Vector2& vcw,
    const bool& vcl, const Ogre::Real& vclat, const Ogre::Vector3& vclc, 
    const Ogre::Real& vcltm)
    : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt), 
    layeredClouds(lc), volumetricClouds(vc), vcWindSpeed(vcws)
    , vcAutoupdate(vcauto), vcWindDir(vcwd), vcAmbientColor(vcac), 
    vcLightResponse(vclr), vcAmbientFactors(vcaf), vcWheater(vcw)
    , vcLightnings(vcl), vcLightningsAT(vclat), vcLightningsColor(vclc), 
    vcLightningsTM(vcltm)
    {}

    // Skydome + vol. clouds.
    SkyPreset(const Ogre::Vector3 t, const Ogre::Real& tm, 
              const Ogre::Real& mp, 
              const SkyX::AtmosphereManager::Options& atmOpt,
                 const bool& lc, const bool& vc, const Ogre::Real& vcws, 
                 const bool& vcauto, const Ogre::Radian& vcwd,
                 const Ogre::Vector3& vcac, const Ogre::Vector4& vclr, 
                 const Ogre::Vector4& vcaf, const Ogre::Vector2& vcw)
                 : time(t), timeMultiplier(tm), moonPhase(mp), 
                 atmosphereOpt(atmOpt), layeredClouds(lc), volumetricClouds(vc),
                 vcWindSpeed(vcws)
                 , vcAutoupdate(vcauto), vcWindDir(vcwd), vcAmbientColor(vcac), 
                 vcLightResponse(vclr), vcAmbientFactors(vcaf), vcWheater(vcw), 
                 vcLightnings(false)
    {}

    // Skydome settings.
    SkyPreset(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp,
              const SkyX::AtmosphereManager::Options& atmOpt, const bool& lc)
        : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt),
        layeredClouds(lc), volumetricClouds(false), vcLightnings(false)
    {}

    // Time.
    Ogre::Vector3 time;
    // Time multiplier.
    Ogre::Real timeMultiplier;
    // Moon phase.
    Ogre::Real moonPhase;
    // Atmosphere options.
    SkyX::AtmosphereManager::Options atmosphereOpt;
    // Layered clouds?
    bool layeredClouds;
    // Volumetric clouds?
    bool volumetricClouds;
    // VClouds wind speed.
    Ogre::Real vcWindSpeed;
    // VClouds autoupdate.
    bool vcAutoupdate;
    // VClouds wind direction.
    Ogre::Radian vcWindDir;
    // VClouds ambient color.
    Ogre::Vector3 vcAmbientColor;
    // VClouds light response.
    Ogre::Vector4 vcLightResponse;
    // VClouds ambient factors.
    Ogre::Vector4 vcAmbientFactors;
    // VClouds wheater.
    Ogre::Vector2 vcWheater;
    // VClouds lightnings?
    bool vcLightnings;
    // VClouds lightnings average aparition time.
    Ogre::Real vcLightningsAT;
    // VClouds lightnings color.
    Ogre::Vector3 vcLightningsColor;
    // VClouds lightnings time multiplier.
    Ogre::Real vcLightningsTM;
};

// ========================================================================= //

// Define several SkyPresets for use.
SkyPreset SkyPresets[] = {
    // Sunset.
    SkyPreset(Ogre::Vector3(8.85f, 7.5f, 20.5f), -0.08f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30.f, Ogre::Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3.f, 4.f), false, true, 300.f, false, Ogre::Radian(270.f), Ogre::Vector3(0.63f, 0.63f, 0.7f), Ogre::Vector4(0.35f, 0.2f, 0.92f, 0.1f), Ogre::Vector4(0.4f, 0.7f, 0.f, 0.f), Ogre::Vector2(0.8f, 1.f)),
    // Clear.
    SkyPreset(Ogre::Vector3(17.16f, 7.5f, 20.5f), 0.f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0017f, 0.000675f, 30.f, Ogre::Vector3(0.57f, 0.54f, 0.44f), -0.991f, 2.5f, 4.f), false),
    // Thunderstorm 1.
    SkyPreset(Ogre::Vector3(12.23f, 7.5f, 20.5f), 0.f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30.f, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 1.f, 4.f), false, true, 300.f, false, Ogre::Radian(0.f), Ogre::Vector3(0.63f, 0.63f, 0.7f), Ogre::Vector4(0.25f, 0.4f, 0.5f, 0.1f), Ogre::Vector4(0.45f, 0.3f, 0.6f, 0.1f), Ogre::Vector2(1.f, 1.f), true, 0.5f, Ogre::Vector3(1.f, 0.976f, 0.92f), 0.2f),
    // Thunderstorm 2.
    SkyPreset(Ogre::Vector3(10.23f, 7.5f, 20.5f), 0.f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30.f, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5f, 4.f), false, true, 300.f, false, Ogre::Radian(0.f), Ogre::Vector3(0.63f, 0.63f, 0.7f), Ogre::Vector4(0.f, 0.02f, 0.34f, 0.24f), Ogre::Vector4(0.29f, 0.3f, 0.6f, 1.f), Ogre::Vector2(1.f, 1.f), true, 0.5f, Ogre::Vector3(0.95f, 1.f, 1.f), 2.f),
    // Desert.
    SkyPreset(Ogre::Vector3(7.59f, 7.5f, 20.5f), 0.f, -0.8f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0072f, 0.000925f, 30.f, Ogre::Vector3(0.71f, 0.59f, 0.53f), -0.997f, 2.5f, 1.f), true),
    // Night.
    SkyPreset(Ogre::Vector3(21.5f, 7.5, 20.5), 0.03f, -0.25f, SkyX::AtmosphereManager::Options(), true)
};

// ========================================================================= //

#endif

// ========================================================================= //