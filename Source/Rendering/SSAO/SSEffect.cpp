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
// File: SSEffect.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements SSEffect class.
// ========================================================================= //

#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRenderTarget.h>
#include <OgrePixelFormat.h>
#include <OgreTextureManager.h>
#include <OgrePass.h>
#include <OgreSceneManager.h>
#include <OgreTexture.h>
//#include <OgreRectangle2D.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRoot.h>
#include <OgreRenderTexture.h>
#include <OgreRenderSystem.h>
#include <OgreManualObject.h>
#include "SSEffect.hpp"
#include <string>

// ========================================================================= //

QuadRenderer::QuadRenderer(Ogre::Camera *c) : quad(NULL), cam(c) 
{
    quad = cam->getSceneManager()->createManualObject("QuadRenderer::quad");

    //! (courtesy of Ogre API docs)
    //! The corners are ordered as follows:
    //! [0] = top-right near
    //! [1] = top-left near
    //! [2] = bottom-left near
    //! [3] = bottom-right near
    //! [4] = top-right far    <--- want
    //! [5] = top-left far     <--- want
    //! [6] = bottom-left far  <--- want
    //! [7] = bottom-right far <--- want
    const Ogre::Vector3 *CORNERS = cam->getWorldSpaceCorners();

    quad->begin("Chuck Norris", Ogre::RenderOperation::OT_TRIANGLE_LIST); {
        //! store frustum corner in normal attribute
        //! and go anti clockwise

        // top-left
        quad->position(Ogre::Vector3(-1, 1, 0));
        quad->normal(CORNERS[5]);
        quad->textureCoord(Ogre::Vector2(0, 0));

        // bottom-left
        quad->position(Ogre::Vector3(-1, -1, 0));
        quad->normal(CORNERS[6]);
        quad->textureCoord(Ogre::Vector2(0, 1));

        // bottom-right
        quad->position(Ogre::Vector3(1, -1, 0));
        quad->normal(CORNERS[7]);
        quad->textureCoord(Ogre::Vector2(1, 1));

        // top-right
        quad->position(Ogre::Vector3(1, 1, 0));
        quad->normal(CORNERS[4]);
        quad->textureCoord(Ogre::Vector2(1, 0));

        // we put the vertices in anti-clockwise,
        // so just start at 0 and go to 3
        quad->quad(0, 1, 2, 3);
    } quad->end();
}

// ========================================================================= //

QuadRenderer::~QuadRenderer(void)
{
    
}

// ========================================================================= //

void QuadRenderer::go(Ogre::Pass *p, Ogre::Viewport *vp) const 
{
    Ogre::RenderSystem *rs = Ogre::Root::getSingleton().getRenderSystem();

    Ogre::RenderOperation rop;
    quad->getSection(0)->getRenderOperation(rop);

    rs->_beginFrame();
    rs->_setViewport(vp);

    Ogre::SceneManager *sceneMgr = quad->_getManager();

    sceneMgr->_setPass(p, true, false);
    if (p->hasVertexProgram())
        rs->bindGpuProgramParameters(
        Ogre::GPT_VERTEX_PROGRAM, p->getVertexProgramParameters(), 0x01);
    else
        rs->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
    if (p->hasFragmentProgram())
        rs->bindGpuProgramParameters(
        Ogre::GPT_FRAGMENT_PROGRAM, p->getFragmentProgramParameters(), 0x01);
    else
        rs->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);

    rs->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
    rs->_setViewMatrix(Ogre::Matrix4::IDENTITY);
    rs->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

    rs->_render(rop);

    rs->_endFrame();
}

// ========================================================================= //

void QuadRenderer::destroy(void)
{
    quad->_getManager()->destroyManualObject(quad);
    quad = NULL;
}

// ========================================================================= //

SSEffect::SSEffect(const std::string &n, QuadRenderer &qr) :
cam(NULL), vp(NULL), tex(NULL), texIsExtern(false), rt(NULL),
pass(NULL), enabled(true), name(n), qr(qr) 
{
    //quad = new Ogre::Rectangle2D(true);
    //quad->setCorners(-1, 1, 1, -1);
}

// ========================================================================= //

SSEffect::~SSEffect(void)
{
    destroy();
    //delete quad;
}

// ========================================================================= //

void SSEffect::destroy(void)
{
    if (!texIsExtern) {
        if (tex)
            Ogre::TextureManager::getSingleton().remove(tex->getName());
        /*if (cam)
            cam->getSceneManager()->destroyCamera(cam);*/
    }

    tex = NULL;
    cam = NULL;
    rt = NULL;
    vp = NULL;
}

// ========================================================================= //

void SSEffect::create(size_t w, size_t h, Ogre::PixelFormat pf) 
{
    destroy();

    Ogre::TexturePtr midPtr = Ogre::TextureManager::getSingleton().createManual(
        name + "_tex", "Internal", Ogre::TEX_TYPE_2D, w, h, 0,
        pf, Ogre::TU_RENDERTARGET);
    tex = midPtr.get(); texIsExtern = false;
    rt = tex->getBuffer(0)->getRenderTarget(0);
    rt->setAutoUpdated(false);

    cam = qr.cam->getSceneManager()->createCamera(name + "_cam");
    vp = rt->addViewport(cam, rt->getNumViewports());
    vp->setBackgroundColour(Ogre::ColourValue::White);
    vp->setClearEveryFrame(false);
    vp->setShadowsEnabled(false);
    vp->setOverlaysEnabled(false);

    cam->setAspectRatio(float(w) / h);
    cam->setFOVy(Ogre::Degree(90));
}

// ========================================================================= //

void SSEffect::create(Ogre::Texture *et) 
{
    destroy();

    tex = et; texIsExtern = true;
    rt = tex->getBuffer(0)->getRenderTarget(0);

    vp = rt->getViewport(0);
    cam = vp->getCamera();
}

// ========================================================================= //

void SSEffect::update(void)
{
    if (!enabled || !pass) return;

    qr.go(pass, vp);
}

// ========================================================================= //

void SSEffect::clear(void)
{
    Ogre::RenderSystem *rs = Ogre::Root::getSingleton().getRenderSystem();
    rs->_setViewport(vp);
    rs->clearFrameBuffer(
        Ogre::FBT_COLOUR | Ogre::FBT_DEPTH | Ogre::FBT_STENCIL,
        vp->getBackgroundColour());
}

// ========================================================================= //