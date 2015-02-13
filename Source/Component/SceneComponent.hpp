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
// File: PositionComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines PositionComponent class.
// ========================================================================= //

#ifndef __SCENECOMPONENT_HPP__
#define __SCENECOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Holds transform information for position in the game world.
class SceneComponent : public Component
{
public:
	// Default initializes Ogre::SceneNode.
	explicit SceneComponent(void);

	// Empty destructor.
	virtual ~SceneComponent(void) override;

	// Creates a Ogre::SceneNode within the world.
	virtual void init(EntityPtr, World&) override;

	// Destroys the internal Ogre::SceneNode.
	virtual void destroy(EntityPtr, World&) override;

	// Empty.
	virtual void update(EntityPtr, World&) override;

	// Empty.
	virtual void message(const ComponentMessage&) override;

	// Component functions:

	// Joins a ModelComponent to itself.
	void attachModel(const ModelComponentPtr);

	// Joins a LightComponent to itself.
	void attachLight(const LightComponentPtr);

	// Sets position of Ogre::SceneNode. 
	// Note: This may be overriden by a PhysicsComponent attached to the
	// same Entity.
	void setPosition(const Ogre::Real, const Ogre::Real, const Ogre::Real);

	// Sets orientation of Ogre::SceneNode.
	void setOrientation(const Ogre::Real, const Ogre::Real, const Ogre::Real,
						const Ogre::Real);

	// Getters:

	// Returns pointer to internal Ogre::SceneNode.
	Ogre::SceneNode* getSceneNode(void) const;

private:
	Ogre::SceneNode* m_node;
};

// ========================================================================= //

inline Ogre::SceneNode* SceneComponent::getSceneNode(void) const{
	return m_node;
}

// ========================================================================= //

#endif

// ========================================================================= //