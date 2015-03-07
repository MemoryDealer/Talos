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
// File: ActorComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ActorComponent class.
// ========================================================================= //

#ifndef __ACTORCOMPONENT_HPP__
#define __ACTORCOMPONENT_HPP__

// ========================================================================= //

#include "SceneComponent.hpp"

// ========================================================================= //
// Allows an Entity to be controlled by either: a player giving local input,
// an AI, a networked player, or from a list of replay commands.
class ActorComponent final : public SceneComponent
{
public:
    // Default initializes scene nodes.
    explicit ActorComponent(void);

    // Empty destructor.
    virtual ~ActorComponent(void) override;

    // Creates all needed scene nodes for controlling first person camera.
    virtual void init(World&) override;

    // Destroys the internal scene nodes for the camera.
    virtual void destroy(World&) override;

    // Empty.
    virtual void update(World&) override;

    // Handles input messages.
    virtual void message(const ComponentMessage&) override;

    // Calls SceneComponent::onComponentAttached() and links camera if the
    // type is CameraComponent.
    virtual void onComponentAttached(ComponentPtr) override;

    // Modes the actor can be in.
    enum class Mode{
        Spectator = 0,
        Player
    };

    // Two types of character controllers.
    enum class CC{
        Kinematic = 0,
        Dynamic
    };

    // Changes the actor's orientation based on relative x/y looking.
    void look(const int, const int);

    // Getters:

    const Ogre::Vector3& getPosition(void) const;

    const Ogre::Quaternion& getOrientation(void) const;

    const bool isMovingForward(void) const;

    const bool isMovingBack(void) const;

    const bool isMovingLeft(void) const;

    const bool isMovingRight(void) const;

    // Setters:

    // Sets the mode of the Actor, affecting controls/movement.
    void setMode(const Mode);

    // Sets translate vector to forward if in spectator mode. 
    void setMovingForward(const bool);

    // Sets translate vector to backward if in spectator mode. 
    void setMovingBack(const bool);

    // Sets translate vector to left if in spectator mode. 
    void setMovingLeft(const bool);

    // Sets translate vector to right if in spectator mode. 
    void setMovingRight(const bool);

private:
    // Ogre3D.
    Ogre::SceneNode* m_rootNode;
    Ogre::SceneNode* m_yawNode;
    Ogre::SceneNode* m_pitchNode;
    Ogre::SceneNode* m_rollNode;
    Ogre::Real m_speed;

    // Character controller.
    CC m_cc;
    union{
        DCC* m_dcc;
        KCC* m_kcc;
    };

    Mode m_mode;
    bool m_movingForward;
    bool m_movingBack;
    bool m_movingLeft;
    bool m_movingRight;
};

// ========================================================================= //

// Getters:

inline const Ogre::Vector3& ActorComponent::getPosition(void) const{
    return m_rootNode->getPosition();
}

inline const Ogre::Quaternion& ActorComponent::getOrientation(void) const{
    return m_yawNode->getOrientation() * m_pitchNode->getOrientation();
}

inline const bool ActorComponent::isMovingForward(void) const{
    return m_movingForward;
}

inline const bool ActorComponent::isMovingBack(void) const{
    return m_movingBack;
}

inline const bool ActorComponent::isMovingLeft(void) const{
    return m_movingLeft;
}

inline const bool ActorComponent::isMovingRight(void) const{
    return m_movingRight;
}

// Setters:

inline void ActorComponent::setMovingForward(const bool moving){
    m_movingForward = moving;
}

inline void ActorComponent::setMovingBack(const bool moving){
    m_movingBack = moving;
}

inline void ActorComponent::setMovingLeft(const bool moving){
    m_movingLeft = moving;
}

inline void ActorComponent::setMovingRight(const bool moving){
    m_movingRight = moving;
}

// ========================================================================= //

#endif

// ========================================================================= //