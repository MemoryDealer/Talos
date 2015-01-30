// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Player.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Player class.
// ========================================================================= //

#include "Player.hpp"
#include "Component/SceneComponent.hpp"
#include "Component/CameraComponent.hpp"

// ========================================================================= //

Player::Player(void) :
m_sceneComponent(new SceneComponent()),
m_cameraComponent(new CameraComponent())
{
	this->attachComponent(m_sceneComponent);
	this->attachComponent(m_cameraComponent);
}

// ========================================================================= //

Player::~Player(void)
{

}

// ========================================================================= //