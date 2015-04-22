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
// File: SkyPresets.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines SkyPresets.
// ========================================================================= //

#include "SkyPresets.hpp"

// ========================================================================= //

// Define several SkyPresets for use.
SkyPreset SkyPresets[6] = {
    // Sunset.
    SkyPreset(Ogre::Vector3(8.85f, 7.5f, 20.5f), -0.08f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30.f, Ogre::Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3.f, 4.f), false, true, 300.f, false, Ogre::Radian(270.f), Ogre::Vector3(0.63f, 0.63f, 0.7f), Ogre::Vector4(0.35f, 0.2f, 0.92f, 0.1f), Ogre::Vector4(0.4f, 0.7f, 0.f, 0.f), Ogre::Vector2(0.8f, 1.f)),
    // Clear.
    SkyPreset(Ogre::Vector3(17.16f, 7.5f, 20.5f), 0.f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0017f, 0.000675f, 30.f, Ogre::Vector3(0.57f, 0.54f, 0.44f), -0.991f, 2.5f, 4.f), false),
    // Thunderstorm 1.
    SkyPreset(Ogre::Vector3(12.23f, 7.5f, 20.5f), 0.f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30.f, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 1.f, 4.f), false, true, 300.f, false, Ogre::Radian(0.f), Ogre::Vector3(0.63f, 0.63f, 0.7f), Ogre::Vector4(0.25f, 0.4f, 0.5f, 0.1f), Ogre::Vector4(0.45f, 0.3f, 0.6f, 0.1f), Ogre::Vector2(1.f, 1.f), true, 0.5f, Ogre::Vector3(1.f, 0.976f, 0.92f), 1.f),
    // Thunderstorm 2.
    SkyPreset(Ogre::Vector3(10.23f, 7.5f, 20.5f), 0.f, 0.f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30.f, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5f, 4.f), false, true, 300.f, false, Ogre::Radian(0.f), Ogre::Vector3(0.63f, 0.63f, 0.7f), Ogre::Vector4(0.f, 0.02f, 0.34f, 0.24f), Ogre::Vector4(0.29f, 0.3f, 0.6f, 1.f), Ogre::Vector2(1.f, 1.f), true, 0.5f, Ogre::Vector3(0.949f, 0.094f, 0.922f), 1.f),
    // Desert.
    SkyPreset(Ogre::Vector3(7.59f, 7.5f, 20.5f), 0.f, -0.8f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0072f, 0.000925f, 30.f, Ogre::Vector3(0.71f, 0.59f, 0.53f), -0.997f, 2.5f, 1.f), true),
    // Night.
    SkyPreset(Ogre::Vector3(21.5f, 7.5, 20.5), 0.03f, -0.25f, SkyX::AtmosphereManager::Options(), true)
};

// ========================================================================= //