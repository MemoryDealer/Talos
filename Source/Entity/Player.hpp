// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Player.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Player class.
// ========================================================================= //

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

// ========================================================================= //

#include "Entity.hpp"

// ========================================================================= //
// 
class Player : public Entity
{
public:
	explicit Player(void);

	virtual ~Player(void) override;

	void init(World& world);

	void destroy(World& world);

	void update(World& world);

private:
	std::shared_ptr<SceneComponent> m_sceneComponent;
	std::shared_ptr<CameraComponent> m_cameraComponent;
};

// ========================================================================= //

#endif

// ========================================================================= //