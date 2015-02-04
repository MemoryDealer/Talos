// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Subject.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Subject class.
// ========================================================================= //

#include "Subject.hpp"

// ========================================================================= //

Subject::Subject(void) :
m_observers()
{
	m_observers.reserve(1);
}

// ========================================================================= //

Subject::~Subject(void)
{

}

// ========================================================================= //

void Subject::notify(const unsigned int id)
{
	for (ObserverList::iterator itr = m_observers.begin();
		 itr != m_observers.end();
		 ++itr){
		(*itr)->onNotify(id);
	}
}

// ========================================================================= //