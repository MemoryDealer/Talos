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
// File: SSEffect.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines SSEffect class.
// ========================================================================= //

#ifndef __SSEFFECT_HPP__
#define __SSEFFECT_HPP__

// ========================================================================= //

#include <string>

#include <OgrePixelFormat.h>
#include <OgreMatrix4.h>

// ========================================================================= //

// courtesy of OgreAutoParamDataSource.cpp
static const Ogre::Matrix4 PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE(
0.5, 0, 0, 0.5,
0, -0.5, 0, 0.5,
0, 0, 1, 0,
0, 0, 0, 1);

namespace Ogre {
    class Viewport;
    class ManualObject;
    class Camera;
    class Pass;
    class Texture;
    class RenderTarget;
}

struct QuadRenderer {
    Ogre::ManualObject *quad;
    Ogre::Camera *cam;

    explicit QuadRenderer(Ogre::Camera *c);
    ~QuadRenderer(void);

    void go(Ogre::Pass *p, Ogre::Viewport *vp) const;
};

// ========================================================================= //

class SSEffect 
{
public:
    explicit SSEffect(const std::string &n, QuadRenderer &qr);
    virtual ~SSEffect(void);

    // render to own target
    virtual void create(size_t w, size_t h, Ogre::PixelFormat pf = Ogre::PF_R8G8B8);
    // render to existing target
    virtual void create(Ogre::Texture *et);
    virtual void destroy(void);

    virtual void update(void);
    virtual void clear(void);

    virtual void enable(bool b) { enabled = b; }

    virtual Ogre::Camera *getCamera(void) const {
        return cam;
    }

    virtual Ogre::Viewport *getViewport(void) const {
        return vp;
    }

    virtual Ogre::Texture *getTexture(void) const {
        return tex;
    }

    virtual Ogre::RenderTarget *getRT(void) const {
        return rt;
    }

    virtual Ogre::Pass *getPass(void) const {
        return pass;
    }

    virtual void setPass(Ogre::Pass *p) {
        pass = p;
    }

    const std::string &getName(void) const {
        return name;
    }

protected:
    Ogre::Camera *cam;
    Ogre::Viewport *vp;
    Ogre::Texture *tex;
    bool texIsExtern;
    Ogre::RenderTarget *rt;

    Ogre::Pass *pass;
    bool enabled;

    std::string name;

    QuadRenderer &qr;
};

// ========================================================================= //

#endif

// ========================================================================= //
