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
	this->addComponent(m_sceneComponent);
	this->addComponent(m_cameraComponent);
}

// ========================================================================= //

Player::~Player(void)
{

}

// ========================================================================= //

void Player::init(World& world)
{
	m_cameraComponent->init(world);
}

// ========================================================================= //

void Player::destroy(World& world)
{
	m_cameraComponent->destroy(world);
}

// ========================================================================= //

void Player::update(World& world)
{
	m_cameraComponent->update();
}

// ========================================================================= //