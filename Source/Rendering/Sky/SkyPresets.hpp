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

#include "Rendering/Sky/SkyX/SkyX.h"
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

extern SkyPreset SkyPresets[6];

// ========================================================================= //

#endif

// ========================================================================= //