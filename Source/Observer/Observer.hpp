// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Observer.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Observer class.
// ========================================================================= //

#ifndef __OBSERVER_HPP__
#define __OBSERVER_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

class Observer;

typedef Observer* ObserverPtr;
typedef std::vector<ObserverPtr> ObserverList;

// ========================================================================= //

class Observer
{
public:
	explicit Observer(void) { }

	virtual ~Observer(void) = 0 { }

	virtual void onNotify(const unsigned int) = 0;
};

// ========================================================================= //

#endif

// ========================================================================= //