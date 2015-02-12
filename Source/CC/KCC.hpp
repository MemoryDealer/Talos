// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: KCC.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines KCC class.
// ========================================================================= //

#ifndef __KCC_HPP__
#define __KCC_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

using namespace physx;

// ========================================================================= //

class World;

// ========================================================================= //
// Kinematic character controller using PhysX.
class KCC final
{
public:
	explicit KCC(void);

	~KCC(void);

	bool init(World&);

	void destroy(World&);

	PxExtendedVec3 update(const PxReal, const PxReal, const PxReal);

	// Setters:

	void setPosition(const PxReal, const PxReal, const PxReal);

	void setPosition(const Ogre::Vector3&);

private:
	PxController* m_controller;
};

// ========================================================================= //

// Setters:

inline void KCC::setPosition(const PxReal x, 
							 const PxReal y, 
							 const PxReal z){
	m_controller->setPosition(PxExtendedVec3(x, y, z));
}

inline void KCC::setPosition(const Ogre::Vector3& p){
	this->setPosition(p.x, p.y, p.z);
}

// ========================================================================= //

#endif

// ========================================================================= //