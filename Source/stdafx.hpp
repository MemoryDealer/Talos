// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: stdafx.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// A single point of access for needed headers.
// ========================================================================= //

#ifndef __STDAFX_HPP__
#define __STDAFX_HPP__

// ========================================================================= //

// C++.
#include <list>
#include <map>
#include <stack>

// Ogre3D.
#include <Ogre.h>

// SDL.
#include <SDL.h>
#include <SDL_syswm.h>

// CEGUI.
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

// PhysX
#define Physx physx
#include <PxPhysicsAPI.h>

// My own files.
#include "Core/HelperFunctions.hpp"
#include "Core/Assert.hpp"

// ========================================================================= //

#endif

// ========================================================================= //