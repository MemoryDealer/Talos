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
m_origin(Ogre::Vector3::ZERO),
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
	m_points.push_back(m_origin + p);
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::addPoint(const Ogre::Real x, 
							const Ogre::Real y, 
							const Ogre::Real z)
{
	this->addPoint(Ogre::Vector3(x, y, z));
}

// ========================================================================= //

void DynamicLines::setPoint(const unsigned short index, 
							const Ogre::Vector3& p)
{
	Assert(index < m_points.size(), "Point index out of bounds!");

	m_points[index] = m_origin + p;
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::translate(const Ogre::Vector3& offset)
{
	m_origin += offset;
	for (std::vector<Ogre::Vector3>::iterator itr = m_points.begin();
		 itr != m_points.end();
		 ++itr){
		*itr += offset;
	}

	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::translate(const Ogre::Real dx, 
							 const Ogre::Real dy, 
							 const Ogre::Real dz)
{
	this->translate(Ogre::Vector3(dx, dy, dz));
}

// ========================================================================= //

void DynamicLines::clear(void)
{
	m_points.clear();
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::setOrigin(const Ogre::Vector3& origin)
{
	// Reset all points to zero origin.
	for (std::vector<Ogre::Vector3>::iterator itr = m_points.begin();
		 itr != m_points.end();
		 ++itr){
		*itr -= m_origin;
		*itr += origin;
	}

	m_origin = origin;
	m_dirty = true;
}

// ========================================================================= //

void DynamicLines::setOrigin(const Ogre::Real x, 
							 const Ogre::Real y, 
							 const Ogre::Real z)
{
	this->setOrigin(Ogre::Vector3(x, y, z));
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

void DynamicLines::setColour(const Colour colour)
{
	switch (colour){
	default:
		break;

		case Colour::WHITE:
			this->setMaterial("BaseWhiteNoLighting");
			break;

		case Colour::RED:
			this->setMaterial("BaseRedNoLighting");
			break;
	}
}

// ========================================================================= //