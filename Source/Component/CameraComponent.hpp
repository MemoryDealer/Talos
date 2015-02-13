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
// File: CameraComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines CameraComponent class.
// ========================================================================= //

#ifndef __CAMERACOMPONENT_HPP__
#define __CAMERACOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Holds an Ogre::Camera to view the game world.
class CameraComponent : public Component
{
public:
	// Sets name to "CameraComponent".
	explicit CameraComponent(void);

	// Empty destructor.
	virtual ~CameraComponent(void) override;

	// Creates Ogre::Camera object, sets its clip distance, sets the aspect
	// ratio, wires up the camera to the viewport.
	virtual void init(EntityPtr, World&) override;

	// Destroys the Ogre::Camera object.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

	// Getters: 

	// Returns pointer to internal Ogre::Camera.
	Ogre::Camera* getCamera(void) const;

private:
	Ogre::Camera* m_camera;
};

// ========================================================================= //

inline Ogre::Camera* CameraComponent::getCamera(void) const{
	return m_camera;
}

// ========================================================================= //

#endif

// ========================================================================= //