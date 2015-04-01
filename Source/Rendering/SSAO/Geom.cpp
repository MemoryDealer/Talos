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
// File: Geom.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Geom class.
// ========================================================================= //

#include "Geom.hpp"
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>

// ========================================================================= //

Geom::Geom(const std::string &n, QuadRenderer &qr) : SSEffect(n, qr) 
{

}

// ========================================================================= //

Geom::~Geom(void) 
{

}

// ========================================================================= //

void Geom::create(size_t w, size_t h, Ogre::PixelFormat pf) 
{
    SSEffect::create(w, h, pf);
}

// ========================================================================= //

void Geom::update(void) 
{
    if (!enabled) return;

    vp->setMaterialScheme("geom");
    Ogre::MaterialManager::getSingleton().setActiveScheme("geom");

    // render using the geom scheme to our viewport
    // using the main scene camera
    qr.cam->getSceneManager()->_renderScene(qr.cam, vp, false);

    SSEffect::update();
}


// ========================================================================= //