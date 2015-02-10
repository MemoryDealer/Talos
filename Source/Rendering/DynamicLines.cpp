// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: DynamicLines.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements DynamicLines class.
// ========================================================================= //

#include "DynamicLines.hpp"

// ========================================================================= //

enum{
	POSITION_BINDING,
	TEXCOORD_BINDING
};

// ========================================================================= //

DynamicLines::DynamicLines(const Ogre::RenderOperation::OperationType type) :
m_points(),
m_dirty(true)
{
	this->init(type, false);
	this->setMaterial("BaseWhiteNoLighting");
}

// ========================================================================= //

DynamicLines::~DynamicLines(void)
{

}

// ========================================================================= //

void DynamicLines::addPoint(const Ogre::Vector3& p)
{
	m_points.push_back(p);
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::addPoint(const Ogre::Real x, 
							const Ogre::Real y, 
							const Ogre::Real z)
{
	m_points.push_back(Ogre::Vector3(x, y, z));
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::setPoint(const unsigned short index, 
							const Ogre::Vector3& p)
{
	Assert(index < m_points.size(), "Point index out of bounds!");

	m_points[index] = p;
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::clear(void)
{
	m_points.clear();
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::update(void)
{
	if (m_dirty){
		this->fillHardwareBuffers();
	}
}

// ========================================================================= //

void DynamicLines::createVertexDeclaration(void)
{
	Ogre::VertexDeclaration* dec = mRenderOp.vertexData->vertexDeclaration;
	dec->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
}

// ========================================================================= //

void DynamicLines::fillHardwareBuffers(void)
{
	const int size = m_points.size();

	this->prepareHardwareBuffers(size, 0);

	if (!size){
		mBox.setExtents(Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
		m_dirty = false;
		return;
	}

	Ogre::Vector3 aabbMin = m_points[0];
	Ogre::Vector3 aabbMax = m_points[0];

	Ogre::HardwareVertexBufferSharedPtr vbuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	Ogre::Real* pos = static_cast<Ogre::Real*>(
		vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	for (int i = 0; i < size; ++i){
		*pos++ = m_points[i].x;
		*pos++ = m_points[i].y;
		*pos++ = m_points[i].z;

		// Change minimum values if necessary.
		if (m_points[i].x < aabbMin.x){
			aabbMin.x = m_points[i].x;
		}
		if (m_points[i].y < aabbMin.y){
			aabbMin.y = m_points[i].y;
		}
		if (m_points[i].z < aabbMin.z){
			aabbMin.z = m_points[i].z;
		}

		// Change maximum values if necessary.
		if (m_points[i].x > aabbMax.x){
			aabbMax.x = m_points[i].x;
		}
		if (m_points[i].y > aabbMax.y){
			aabbMax.y = m_points[i].y;
		}
		if (m_points[i].z > aabbMax.z){
			aabbMax.z = m_points[i].z;
		}
	}

	vbuf->unlock();

	mBox.setExtents(aabbMin, aabbMax);

	m_dirty = false;
}

// ========================================================================= //