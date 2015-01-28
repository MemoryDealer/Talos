// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: IntroState.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines IntroState class.
// ========================================================================= //

#ifndef __INTROSTATE_HPP__
#define __INTROSTATE_HPP__

// ========================================================================= //

#include "EngineState.hpp"

// ========================================================================= //
// A test state for now.
class IntroState : public EngineState
{
public:
	// Calls EngineState constructor.
	explicit IntroState(void);

	// Empty destructor.
	virtual ~IntroState(void) override;

	// Set up basic stuff.
	virtual void enter(void) override;

	// Free basic stuff.
	virtual void exit(void) override;

	// Test.
	virtual void update(void) override;

private:
	Ogre::SceneNode* m_cameraNode;
};

// ========================================================================= //

#endif

// ========================================================================= //