// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: DynamicRenderable.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements DynamicRenderable class.
// ========================================================================= //

#include "DynamicRenderable.hpp"

// ========================================================================= //

DynamicRenderable::DynamicRenderable(void) :
m_vertexBufferCapacity(0),
m_indexBufferCapacity(0)
{

}

// ========================================================================= //

DynamicRenderable::~DynamicRenderable(void)
{

}

// ========================================================================= //

void DynamicRenderable::init(const Ogre::RenderOperation::OperationType type,
							 const bool useIndices)
{
	// Initialize render operation.
	mRenderOp.operationType = type;
	mRenderOp.useIndexes = useIndices;
	mRenderOp.vertexData = new Ogre::VertexData();
	if (mRenderOp.useIndexes){
		mRenderOp.indexData = new Ogre::IndexData();
	}

	this->createVertexDeclaration();
}

// ========================================================================= //

void DynamicRenderable::prepareHardwareBuffers(const size_t vertexCount,
											   const size_t indexCount)
{
	// Prepare vertex buffer.
	size_t newVertexCapacity = m_vertexBufferCapacity;
	if ((vertexCount > m_vertexBufferCapacity) ||
		!m_vertexBufferCapacity){
		// vertexCount exceeds current capacity!
		// Reallocate the buffer.
		if (!newVertexCapacity){
			newVertexCapacity = 1;
		}

		// Make capacity the next power of two.
		while (newVertexCapacity < vertexCount){
			newVertexCapacity <<= 1;
		}
	}
	else if (vertexCount < m_vertexBufferCapacity >> 1){
		// Make capacity the previous power of two.
		while(vertexCount < newVertexCapacity >> 1){
			newVertexCapacity >>= 1;
		}
	}
	
	if (newVertexCapacity != m_vertexBufferCapacity){
		m_vertexBufferCapacity = newVertexCapacity;

		// Create new vertex buffer.
		Ogre::HardwareVertexBufferSharedPtr vbuf =
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			mRenderOp.vertexData->vertexDeclaration->getVertexSize(0),
			m_vertexBufferCapacity,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);

		// Bind the buffer.
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);
	}

	// Update vertex count in render operation.
	mRenderOp.vertexData->vertexCount = vertexCount;

	if (mRenderOp.useIndexes){
		OgreAssert(indexCount <= std::numeric_limits<unsigned short>::max(),
				   "indexCount exceeds 16 bit");

		size_t newIndexCapacity = m_indexBufferCapacity;
		// Prepare index buffer.
		if ((indexCount > newIndexCapacity) ||
			!newIndexCapacity){
			// indexCount exceeds current capacity!
			// Reallocate the buffer.

			// Check if this is the first call.
			if (!newIndexCapacity){
				newIndexCapacity = 1;
			}

			// Make capacity the next power of two.
			while (newIndexCapacity < indexCount){
				newIndexCapacity <<= 1;
			}
		}
		else if (indexCount < newIndexCapacity >> 1){
			// Make capacity the previous power of two.
			while (indexCount < newIndexCapacity >> 1){
				newIndexCapacity >>= 1;
			}
		}

		if (newIndexCapacity != m_indexBufferCapacity){
			m_indexBufferCapacity = newIndexCapacity;

			// Create new index buffer.
			mRenderOp.indexData->indexBuffer =
				Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
				Ogre::HardwareIndexBuffer::IT_16BIT,
				m_indexBufferCapacity,
				Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		}

		// Update index count in the render operation.
		mRenderOp.indexData->indexCount = indexCount;
	}
}

// ========================================================================= //

Ogre::Real DynamicRenderable::getBoundingRadius(void) const
{
	return Ogre::Math::Sqrt(std::max(mBox.getMaximum().squaredLength(),
		mBox.getMinimum().squaredLength()));
}

// ========================================================================= //

Ogre::Real DynamicRenderable::getSquaredViewDepth(
	const Ogre::Camera* camera) const
{
	Ogre::Vector3 min, max, mid, dist;
	min = mBox.getMinimum();
	max = mBox.getMaximum();
	mid = ((max - min) * 0.5) + min;
	dist = camera->getDerivedPosition() - mid;

	return dist.squaredLength();
}

// ========================================================================= //