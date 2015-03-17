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
// File: ActorComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements ActorComponent class.
// ========================================================================= //

#include "ActorComponent.hpp"
#include "CameraComponent.hpp"
#include "CC/DCC.hpp"
#include "CC/KCC.hpp"
#include "ComponentMessage.hpp"
#include "ModelComponent.hpp"
#include "Physics/PScene.hpp"
#include "World/World.hpp"

// ========================================================================= //

ActorComponent::ActorComponent(void) :
SceneComponent(),
m_rootNode(nullptr),
m_yawNode(nullptr),
m_pitchNode(nullptr),
m_rollNode(nullptr),
m_yawOrientation(Ogre::Quaternion::IDENTITY),
m_pitchOrientation(Ogre::Quaternion::IDENTITY),
m_translate(Ogre::Vector3::ZERO),
m_speed(0.20f),
m_cc(CC::Kinematic),
m_dcc(nullptr),
m_kcc(nullptr),
m_mode(Mode::Player)
{
    
}

// ========================================================================= //

ActorComponent::~ActorComponent(void)
{

}

// ========================================================================= //

void ActorComponent::init(World& world)
{
    SceneComponent::init(world);

    // Acquire the camera node from the parent SceneComponent class.
    m_rootNode = this->getSceneNode();

    // Create yaw node as the camera's top node.
    m_yawNode = m_rootNode->createChildSceneNode();

    // Create pitch node as the camera's middle node.
    m_pitchNode = m_yawNode->createChildSceneNode();

    // Create roll node as the camera's bottom node.
    m_rollNode = m_pitchNode->createChildSceneNode();

    // Set to identity rotations.
    m_yawNode->setOrientation(Ogre::Quaternion::IDENTITY);
    m_pitchNode->setOrientation(Ogre::Quaternion::IDENTITY);
    m_rollNode->setOrientation(Ogre::Quaternion::IDENTITY);

    // Create PhysX character controller.
    if (m_cc == CC::Kinematic){
        m_kcc = new KCC();
        m_kcc->init(world);
    }
}

// ========================================================================= //

void ActorComponent::destroy(World& world)
{
    // Destroy character controller.
    if (m_cc == CC::Kinematic){
        delete m_kcc;
    }
    else if (m_cc == CC::Dynamic){
        delete m_dcc;
    }

    world.getSceneManager()->destroySceneNode(m_yawNode);
    SceneComponent::destroy(world);
}

// ========================================================================= //

void ActorComponent::update(World& world)
{
    switch (m_mode){
    default:
        break;

    case Mode::Spectator:
        
        break;

    case Mode::Player:
        {
            if (m_cc == CC::Kinematic){
                PxExtendedVec3 pos = m_kcc->update(world);
                m_rootNode->setPosition(Ogre::Real(pos.x),
                                          Ogre::Real(pos.y),
                                          Ogre::Real(pos.z));
            }
        }
        break;
    }
}

// ========================================================================= //

void ActorComponent::message(ComponentMessage& msg)
{
    switch (msg.type){
    default:
        break;

    case ComponentMessage::Type::GetPosition:       
        msg.data = this->getPosition();
        break;

    case ComponentMessage::Type::SetPosition:
        this->setPosition(boost::get<Ogre::Vector3>(msg.data));
        break;

    case ComponentMessage::Type::GetOrientation:
        msg.data = this->getOrientation();
        break;

    case ComponentMessage::Type::SetOrientation:
        this->setOrientation(boost::get<Ogre::Quaternion>(msg.data));
        break;

    case ComponentMessage::Type::Look:
        {
            MouseMove mm = boost::get<MouseMove>(msg.data);
            this->look(mm.relx, mm.rely);
        }
        break;
        
    case ComponentMessage::Type::TransformUpdate:        
        {
            // Apply transform update.
            TransformUpdate transform = 
                boost::get<TransformUpdate>(msg.data);

            /*Ogre::Vector3 src = this->getPosition();
            Ogre::Vector3 dst = transform.position;         
            const Ogre::Real max = 25.f;
            printf("Squared dist: %.2f\n", src.squaredDistance(dst));
            if (src.squaredDistance(dst) < max){
                m_rootNode->translate((dst - src) * 0.1f);
            }
            else{*/
                this->setPosition(transform.position);
                //m_rootNode->_setDerivedOrientation(transform.orientation);
                //m_yawNode->setOrientation(transform.orientation);
                //m_pitchNode->setOrientation(transform.orientation2);
            //}
        }
        break;

    case ComponentMessage::Type::Command:       
        this->applyInput(boost::get<CommandType>(msg.data));
        break;
    }
}

// ========================================================================= //

void ActorComponent::attachCamera(Ogre::Camera* camera)
{
    m_rollNode->attachObject(camera);
}

// ========================================================================= //

void ActorComponent::applyInput(const CommandType& type)
{
    Ogre::Vector3 translate(Ogre::Vector3::ZERO);
    const Ogre::Real move = 1.f;

    // Determine which direction to move, or action to perform.
    switch (type){
    default:
        break;

    case CommandType::MoveForward:
        translate.z = -move;
        break;

    case CommandType::MoveBackward:
        translate.z = move;
        break;

    case CommandType::MoveRight:
        translate.x = move;
        break;

    case CommandType::MoveLeft:
        translate.x = -move;
        break;

    case CommandType::Jump:
        m_kcc->jump();
        return;
    }

    // Calculate movement vector.
    translate = m_yawOrientation * m_pitchOrientation * translate;

    // Calculate the forwards vector and use it to keep the player moving at
    // the same velocity despite the pitch of the camera.
    Ogre::Vector3 right, up, forwards;
    m_rollNode->_getDerivedOrientation().ToAxes(right, up, forwards);
    up.crossProduct(right);
    up.normalise();

    // Modify original movement vector.
    translate.x /= up.y;
    translate.z /= up.y;

    // Prevent faster movement when moving diagonally.
    translate.normalise();

    // Apply actor's speed.
    translate *= m_speed;

    switch (m_mode){
    default:
        break;

    case Mode::Player:
        {
            // Update kinematic controller.
            translate.y = 0.f;
            PxExtendedVec3 pos = m_kcc->move(translate);

            // Update scene node with controller's new position.
            m_rootNode->setPosition(Ogre::Real(pos.x),
                                    Ogre::Real(pos.y),
                                    Ogre::Real(pos.z));
        }
        break;

    case Mode::Spectator:
        m_rootNode->translate(translate, Ogre::SceneNode::TS_LOCAL);
        break;
    }    
}

// ========================================================================= //

void ActorComponent::look(const int relx, const int rely)
{
    if (relx == 0 && rely == 0){
        return;
    }

    const Ogre::Real sens = 0.2f;

    m_yawNode->yaw(Ogre::Degree(-Ogre::Real(relx) * sens));
    m_pitchNode->pitch(Ogre::Degree(-Ogre::Real(rely) * sens));
    {
        // Prevent the camera from pitching upside down.
        Ogre::Real pitchAngle = 0.f;
        Ogre::Real pitchAngleParity = 0.f;

        // Get the angle of rotation around the x-axis.
        pitchAngle = (2.f * Ogre::Degree(Ogre::Math::ACos(
            m_pitchNode->getOrientation().w)).valueDegrees());

        pitchAngleParity = m_pitchNode->getOrientation().x;

        // Limit pitch.
        // @TODO: Define this in data.
        if (pitchAngle > 80.f){
            if (pitchAngleParity > 0){
                m_pitchNode->setOrientation(Ogre::Quaternion(
                    Ogre::Math::Sqrt(0.5f),
                    Ogre::Math::Sqrt(0.5f) - 0.115f,
                    0.f,
                    0.f));
            }
            else if (pitchAngleParity < 0){
                m_pitchNode->setOrientation(Ogre::Quaternion(
                    Ogre::Math::Sqrt(0.5f),
                    -Ogre::Math::Sqrt(0.5f) + 0.115f,
                    0.f,
                    0.f));
            }
        }
    }

    // Store orientations for next call to applyInput().
    m_yawOrientation = m_yawNode->getOrientation();
    m_pitchOrientation = m_pitchNode->getOrientation();
}

// ========================================================================= //

// Getters:

// ========================================================================= //

const Ogre::Vector3& ActorComponent::getPosition(void) const{
    return m_rootNode->getPosition();
}

// ========================================================================= //

const Ogre::Quaternion& ActorComponent::getOrientation(void) const{
    return m_rollNode->_getDerivedOrientation();    
}

// ========================================================================= //

const Ogre::Quaternion& ActorComponent::getYawOrientation(void) const
{
    return m_yawNode->getOrientation();
}

// ========================================================================= //

const Ogre::Quaternion& ActorComponent::getPitchOrientation(void) const
{
    return m_pitchNode->getOrientation();
}

// ========================================================================= //

// Setters:

// ========================================================================= //

void ActorComponent::setPosition(const Ogre::Vector3& pos)
{
    SceneComponent::setPosition(pos);
    m_kcc->setPosition(pos);
}

// ========================================================================= //

void ActorComponent::setOrientation(const Ogre::Quaternion& orientation)
{
    m_rollNode->_setDerivedOrientation(orientation);
}

// ========================================================================= //

void ActorComponent::setYawOrientation(const Ogre::Quaternion& orientation)
{
    m_yawOrientation = orientation;
}

// ========================================================================= //

void ActorComponent::setPitchOrientation(const Ogre::Quaternion& orientation)
{
    m_pitchOrientation = orientation;
}

// ========================================================================= //

void ActorComponent::setMode(const Mode mode)
{
    m_mode = mode;

    switch (m_mode){
    default:
        break;

    case Mode::Player:
        // For debugging.
        m_kcc->setPosition(m_rootNode->getPosition());
        break;

    case Mode::Spectator:

        break;
    }
}

// ========================================================================= //