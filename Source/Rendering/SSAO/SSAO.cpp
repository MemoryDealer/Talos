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
// File: SSAO.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements SSAO class.
// ========================================================================= //

#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include "SSAO.hpp"

// ========================================================================= //

SSAO::SSAO(const std::string &n, QuadRenderer &qr) : SSEffect(n, qr) 
{
    Ogre::MaterialPtr midPtr = (
        Ogre::MaterialManager::getSingleton().load("ssao", "Internal").staticCast<Ogre::Material>());
    
    Ogre::Material *mat = midPtr.get();
    pass = mat->getBestTechnique()->getPass(0);
    pass->getTextureUnitState(0)->setTextureName("geom_tex");
    pass->_load();
}

// ========================================================================= //

SSAO::~SSAO(void) 
{

}

// ========================================================================= //

void SSAO::create(size_t w, size_t h, Ogre::PixelFormat pf) 
{
    SSEffect::create(w, h, pf);
}

// ========================================================================= //

void SSAO::update(void) 
{
    if (!enabled || !pass) return;

    Ogre::GpuProgramParametersSharedPtr params =
        pass->getFragmentProgramParameters();
    if (params->_findNamedConstantDefinition("pMat"))
        params->setNamedConstant(
        "pMat",
        PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE *
        qr.cam->getProjectionMatrixWithRSDepth());
    if (params->_findNamedConstantDefinition("far"))
        params->setNamedConstant("far", qr.cam->getFarClipDistance());

    SSEffect::update();
}

// ========================================================================= //