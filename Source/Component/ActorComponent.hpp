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

#include "Command/CommandTypes.hpp"
#include "SceneComponent.hpp"

// ========================================================================= //

struct ActorState{
    explicit ActorState(void) : 
    position(Ogre::Vector3::ZERO),
    orientation(Ogre::Quaternion::IDENTITY),
    orientation2(Ogre::Quaternion::IDENTITY)
    {
        memset(&input, 0, sizeof(input));
    }

    Ogre::Vector3 position;
    Ogre::Quaternion orientation, orientation2;
    struct{
        bool forward, back, right, left;
    } input;
};

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
    virtual void init(World& world) override;

    // Destroys the internal scene nodes for the camera.
    virtual void destroy(World& world) override;

    // Empty.
    virtual void update(World& world) override;

    // Handles input messages.
    virtual void message(ComponentMessage& msg) override;

    // Attaches Ogre::Camera to roll node.
    virtual void attachCamera(Ogre::Camera* camera) override;

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

    void applyInput(const CommandType& type);

    // Changes the actor's orientation based on relative x/y looking.
    void look(const int relx, const int rely);

    // Getters:

    const Ogre::Vector3& getPosition(void) const;

    const Ogre::Quaternion& getOrientation(void) const;

    const ActorState& getState(void) const;

    // Setters:

    virtual void setPosition(const Ogre::Vector3& pos) override;

    void setState(const ActorState& state);

    // Sets the mode of the Actor, affecting controls/movement.
    void setMode(const Mode);

    // Sets the internal remote flag. Should be set to true for any 
    // instance of a remote player on a client game.
    void setRemote(const bool remote);

    void setWorld(World* world){
        m_world = world;
    }

private:
    // Ogre3D.
    Ogre::SceneNode* m_rootNode;
    Ogre::SceneNode* m_yawNode;
    Ogre::SceneNode* m_pitchNode;
    Ogre::SceneNode* m_rollNode;
    Ogre::Real m_speed;
    bool m_remote; // Is this actor remote (for clients).

    // Character controller.
    CC m_cc;
    union{
        DCC* m_dcc;
        KCC* m_kcc;
    };

    Mode m_mode;
    ActorState m_state;
    World* m_world;
};

// ========================================================================= //

// Getters:

inline const ActorState& ActorComponent::getState(void) const{
    return m_state;
}

// Setters:

inline void ActorComponent::setState(const ActorState& state){
    m_state = state;
}

inline void ActorComponent::setRemote(const bool remote){
    m_remote = remote;
}

// ========================================================================= //

#endif

// ========================================================================= //