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
    virtual void init(void) override;

    // Destroys the internal scene nodes for the camera.
    virtual void destroy(void) override;

    // Empty.
    virtual void update(void) override;

    // Handles input messages.
    virtual void message(ComponentMessage& msg) override;

    // Enums:

    // Modes the actor can be in.
    enum class Mode{
        Player,
        Spectator        
    };

    // Two types of character controllers.
    enum class CC{
        Kinematic,
        Dynamic
    };

    // Component functions:

    // Attaches Ogre::Camera to roll node.
    virtual void attachCamera(Ogre::Camera* camera) override;

    // Applies a translation or action to the actor based on type of input.
    void applyInput(const CommandType& type);

    // Changes the actor's orientation based on relative x/y looking.
    void look(const int relx, const int rely);

    // Casts a ray forward from the player, sending an action message to the 
    // receiving entity, if one is hit and within range.
    void action(void);

    // Getters:

    // Returns position of actor's root scene node.
    const Ogre::Vector3& getPosition(void) const;

    // Returns derived orientation of actor's roll node.
    const Ogre::Quaternion& getOrientation(void) const;

    // Returns orientation of yaw node.
    const Ogre::Quaternion& getYawOrientation(void) const;

    // Returns orientation of pitch node.
    const Ogre::Quaternion& getPitchOrientation(void) const;

    // Setters:

    // Sets position of actor's root node and character controller's position.
    virtual void setPosition(const Ogre::Vector3& pos) override;

    // Sets derived orientation of roll node.
    virtual void setOrientation(const Ogre::Quaternion& orientation) override;

    // Sets yaw node's orientation.
    void setYawOrientation(const Ogre::Quaternion& orientation);

    // Sets pitch node's orientation.
    void setPitchOrientation(const Ogre::Quaternion& orientation);

    // Sets the mode of the Actor, affecting controls/movement.
    void setMode(const Mode& mode);

private:
    // Scene node created from parent scene component.
    Ogre::SceneNode* m_rootNode;

    // Three axis nodes for camera movement.
    Ogre::SceneNode* m_yawNode;
    Ogre::SceneNode* m_pitchNode;
    Ogre::SceneNode* m_rollNode;

    // Store yaw/pitch for applying input from network component.
    Ogre::Quaternion m_yawOrientation;
    Ogre::Quaternion m_pitchOrientation;

    // The amount the actor move each frame.
    Ogre::Vector3 m_translate;

    // Speed multiplier for affecting movement.
    Ogre::Real m_speed;

    // Character controller type.
    CC m_cc;

    // Use either a kinematic or dynamic character controller.
    // @TODO: use an interface.
    union{
        DCC* m_dcc;
        KCC* m_kcc;
    };

    // Current mode of actor.
    Mode m_mode;
};

// ========================================================================= //

#endif

// ========================================================================= //