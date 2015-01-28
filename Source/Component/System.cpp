// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: System.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements System class.
// ========================================================================= //

#include "System.hpp"

// ========================================================================= //

template<typename T>
System::System(const unsigned int size) :
m_components(),
m_numActive(0)
{
	m_components = new T[size];
}

// ========================================================================= //

System::~System(void)
{
	
}

// ========================================================================= //

void System::update(void)
{
	for (unsigned int i = 0; i < m_numActive; ++i){
		
	}
}

// ========================================================================= //