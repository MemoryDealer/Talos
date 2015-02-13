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
// File: DynamicRenderable.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines DynamicRenderable class.
// ========================================================================= //

#ifndef __DYNAMICRENDERABLE_HPP__
#define __DYNAMICRENDERABLE_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //
// Abstract base class for dynamically growing hardware buffers.
class DynamicRenderable : public Ogre::SimpleRenderable
{
public:
	// Empty constructor.
	explicit DynamicRenderable(void);

	// Empty destructor.
	virtual ~DynamicRenderable(void) = 0;

	// Initializes render operations, and calls createVertexDeclaration().
	void init(const Ogre::RenderOperation::OperationType, const bool);

	// Overrides Ogre::SimpleRenderable.
	virtual Ogre::Real getBoundingRadius(void) const override;

	// Overrides Ogre::SimpleRenderable.
	virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera*) const override;

protected:
	// Maximum capacity of the currently allowed vertex buffer.
	size_t m_vertexBufferCapacity;
	// Maximum capacity of the currently allowed index buffer;
	size_t m_indexBufferCapacity;

	// Creates the vertex declaration.
	virtual void createVertexDeclaration(void) = 0;

	// Prepares hardware buffers for requested vertex/index counts. Must be 
	// called before locking the buffers in fillHardwareBuffers().
	void prepareHardwareBuffers(const size_t, const size_t);

	// Fills the hardware vertex/index buffers with data.
	virtual void fillHardwareBuffers(void) = 0;
};

// ========================================================================= //

#endif

// ========================================================================= //