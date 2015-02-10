// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
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